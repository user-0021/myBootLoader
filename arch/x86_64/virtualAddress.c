#include <mem/virtualAddress.h>
#include <uefi/lib/consoleio.h>

#define ENTRY_PRESENT(b) 				(((b) & 1UL) << 0)					//このページはメモリ内にある
#define ENTRY_WRITABLE(b) 				(((b) & 1UL) << 1)					//このページへの書き込みは許可されている
#define ENTRY_USER_ACCESSIBLE(b) 		(((b) & 1UL) << 2)					//0の場合、カーネルモードのみこのページにアクセスできる
#define ENTRY_WRITE_THROUGH_CACHING(b) 	(((b) & 1UL) << 3)					//書き込みはメモリに対して直接行われる
#define ENTRY_DISABLE_CACHE(b)			(((b) & 1UL) << 4)					//このページにキャッシュを使わない
#define ENTRY_ACCESSED(b) 				(((b) & 1UL) << 5)					//このページが使われているとき、CPUはこのビットを1にする
#define ENTRY_DIRTY(b) 					(((b) & 1UL) << 6)					//このページへの書き込みが行われたとき、CPUはこのビットを1にする
#define ENTRY_HUGE_PAGE(b) 				(((b) & 1UL) << 7)					//P1とP4においては0で、P3においては1GiBのページを、P2においては2MiBのページを作る
#define ENTRY_GROABAL(b) 				(((b) & 1UL) << 8)					//キャッシュにあるこのページはアドレス空間変更の際に初期化されない（CR4レジスタのPGEビットが1である必要がある）
#define ENTRY_LAVAILEBLE(data)			(((data) & 7UL) << 9)					//OSが自由に使える
#define ENTRY_PHYSICAL_ADDRESS(address)	(((address) & 0xFFFFFFFFFF000UL))	//ページ単位にアラインされた、フレームまたは次のページテーブルの52bit物理アドレス
#define ENTRY_HAVAILEBLE(data)			(((data) & 0x7FUL) << 52)				//OSが自由に使える
#define ENTRY_NO_EXECUTEBLE(b)			(((b) & 0x1UL) << 63)					//このページにおいてプログラムを実行することを禁じる（EFERレジスタのNXEビットが1である必要がある）

#define VADDRESS_GET_LEV4_OFFSET(address)	(((address) >> 39) & 0x1FF)
#define VADDRESS_GET_LEV3_OFFSET(address)	(((address) >> 30) & 0x1FF)
#define VADDRESS_GET_LEV2_OFFSET(address)	(((address) >> 21) & 0x1FF)
#define VADDRESS_GET_LEV1_OFFSET(address)	(((address) >> 12) & 0x1FF)
#define VADDRESS_GET_PAGE_OFFSET(address)	(((address) >>  0) & 0xFFF)

#define ADDRESS_IS_ALIGNE_512GIB(address)	    (((address) & 0x7FFFFFFFFF) == 0)



EFI_STATUS allocatePageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev4_page,UINT16 table_offset,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag);

EFI_PHYSICAL_ADDRESS createVirtualMap(EFI_BOOT_SERVICES* bootServices,EFI_LOADED_IMAGE_PROTOCOL* kernel){
	EFI_PHYSICAL_ADDRESS lev4_page;
	EFI_STATUS status;

	wcprintf(L"CreateVirtualMap..\r\n\n");

	//allocate lev4 page
	wcprintf(L"Try allocate lev4 table..\r\n");
	status = bootServices->AllocatePages(AllocateAnyPages,EfiLoaderData,1,&lev4_page);
	if(status != EFI_SUCCESS){
		wcprintf(L"...Failed(ErrorCode:%x)\r\n\n",status);
		return (EFI_PHYSICAL_ADDRESS)NULL;
	}else{
		wcprintf(L"...Success\r\n");
		wcprintf(L"Locate on %0x\r\n\n",(UINT64)lev4_page);
	}

	bootServices->SetMem((void*)lev4_page,MEMORY_PAGE_SIZE,0);
	
	//GetMemorySize
	UINTN mapSize = 0;
	UINTN mapKey, descSize;
	UINT32 descVer;
	EFI_PHYSICAL_ADDRESS descripterBuffer = 0;
	EFI_PHYSICAL_ADDRESS memorySize;

	wcprintf(L"Try load availeble memory..\r\n");
	wcprintf(L"Try get memory map..\r\n");
	status = bootServices->GetMemoryMap(&mapSize,(void*)descripterBuffer,&mapKey,&descSize,&descVer);
	if(status == EFI_BUFFER_TOO_SMALL){
		mapSize = (mapSize >> 12) + 2;
		EFI_PHYSICAL_ADDRESS tmp;
		status = bootServices->AllocatePages(AllocateAnyPages,EfiBootServicesData,mapSize,&tmp);
		descripterBuffer = tmp;
		mapSize = mapSize << 12;

		if(status == EFI_SUCCESS)
			status = bootServices->GetMemoryMap(&mapSize,(void*)descripterBuffer,&mapKey,&descSize,&descVer);
	}

	if(status == EFI_SUCCESS){
		UINTN descCount = mapSize / descSize;
		UINT8* desc = (void*)descripterBuffer;
		desc += descSize * (descCount - 1);
		while(((EFI_MEMORY_DESCRIPTOR*)desc)->Type == EfiReservedMemoryType){
			desc -= descSize;
		}

			memorySize = (((EFI_MEMORY_DESCRIPTOR*)desc)->PhysicalStart >> 12) + (((EFI_MEMORY_DESCRIPTOR*)desc)->NumberOfPages - 1);

		wcprintf(L"...Success\r\n");
		wcprintf(L"Memory availeble size:0x%x\r\n",memorySize);
	}else{
		wcprintf(L"...Failed(ErrorCode:%x)\r\n\n",status);
		return (EFI_PHYSICAL_ADDRESS)NULL;
	}


	// memory direct map 
	wcprintf(L"Try allocate direct map table..\r\n");
	status = allocatePageTableLinear(bootServices,lev4_page,VADDRESS_GET_LEV4_OFFSET(MEMORY_DIRECTMAP_HEAD)
		,0,memorySize,ENTRY_PRESENT(1) | ENTRY_WRITABLE(1) | ENTRY_GROABAL(1) | ENTRY_NO_EXECUTEBLE(1));

	if(status != EFI_SUCCESS){
		wcprintf(L"...Failed(ErrorCode:%x)\r\n\n",status);
		return (EFI_PHYSICAL_ADDRESS)NULL;
	}else{
		wcprintf(L"...Success\r\n");
	}

	// kernel text 
	wcprintf(L"Try allocate kernel text table..\r\n");
	status = allocatePageTableLinear(bootServices,lev4_page,VADDRESS_GET_LEV4_OFFSET(MEMORY_KERNEL_HEAD)
		,kernel->ImageBase,kernel->ImageSize,ENTRY_PRESENT(1) | ENTRY_GROABAL(1));

	if(status != EFI_SUCCESS){
		wcprintf(L"...Failed(ErrorCode:%x)\r\n\n",status);
		return (EFI_PHYSICAL_ADDRESS)NULL;
	}else{
		wcprintf(L"...Success\r\n");
	}

	

	return lev4_page;
}


EFI_STATUS allocateLev2PageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev3_page,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag);
EFI_STATUS allocateLev1PageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev2_page,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag);
EFI_STATUS allocateLev0PageTableLinear(EFI_PHYSICAL_ADDRESS lev1_page,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag);

EFI_STATUS allocatePageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev4_page,UINT16 table_offset,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag){
	
	
	EFI_PHYSICAL_ADDRESS lev3_pages;
	UINTN lev3_page_count = ((allocateSize >> 27) & 0x1FF) + 1;
	EFI_STATUS status = bootServices->AllocatePages(AllocateAnyPages,EfiLoaderData,lev3_page_count,&lev3_pages);
	if(status != EFI_SUCCESS)
		return status;

	bootServices->SetMem((void*)lev3_pages,MEMORY_PAGE_SIZE * lev3_page_count,0);


	UINT64 i;
	EFI_PHYSICAL_ADDRESS writeItr = lev4_page + (((UINT32)table_offset) << 3); 
	EFI_PHYSICAL_ADDRESS readItr = physicalBegin;
	EFI_PHYSICAL_ADDRESS pageItr = lev3_pages;
	for(i = 0;i < lev3_page_count;i++){
		//calc
		UINT64 lowerAlocateSize = ((allocateSize >> 27) & 0x1FF) ? 0x7FFFFFF : (allocateSize & 0x7FFFFFF);

		*((UINT64*)(VOID*)writeItr) = (flag & (~0x000FFFFFFFFFF000UL)) | ENTRY_PHYSICAL_ADDRESS(pageItr);
		status = allocateLev2PageTableLinear(bootServices,pageItr,readItr,lowerAlocateSize,flag);
		if(status != EFI_SUCCESS)
			return status;
		
		writeItr += 8;
		readItr  += 0x8000000000;
		pageItr  += MEMORY_PAGE_SIZE;
		allocateSize -= 0x8000000;
	}

	return EFI_SUCCESS;
}

EFI_STATUS allocateLev2PageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev3_page,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag){

	EFI_PHYSICAL_ADDRESS lev2_pages;
	UINTN lev2_page_count = ((allocateSize >> 18) & 0x1FF) + 1;
	EFI_STATUS status = bootServices->AllocatePages(AllocateAnyPages,EfiLoaderData,lev2_page_count,&lev2_pages);
	if(status != EFI_SUCCESS)
		return status;

	bootServices->SetMem((void*)lev2_pages,MEMORY_PAGE_SIZE * lev2_page_count,0);

	wcprintf(L"lev2:%0x\r\n",lev2_page_count);

	UINT64 i;
	EFI_PHYSICAL_ADDRESS writeItr = lev3_page; 
	EFI_PHYSICAL_ADDRESS readItr = physicalBegin;
	EFI_PHYSICAL_ADDRESS pageItr = lev2_pages;
	for(i = 0;i < lev2_page_count;i++){
		//calc
		UINT64 lowerAlocateSize = ((allocateSize >> 18) & 0x1FF) ? 0x3FFFF : (allocateSize & 0x3FFFF);

		*((UINT64*)(VOID*)writeItr) = (flag & (~0x000FFFFFFFFFF000UL)) | ENTRY_PHYSICAL_ADDRESS(pageItr);
		status = allocateLev1PageTableLinear(bootServices,pageItr,readItr,lowerAlocateSize,flag);
		if(status != EFI_SUCCESS)
			return status;

		writeItr += 8;
		readItr  += 0x40000000;
		pageItr  += MEMORY_PAGE_SIZE;
		allocateSize -= 0x40000;
	}

	return EFI_SUCCESS;
}

EFI_STATUS allocateLev1PageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev2_page,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag){

	EFI_PHYSICAL_ADDRESS lev1_pages;
	UINTN lev1_page_count = ((allocateSize >> 9) & 0x1FF) + 1;
	EFI_STATUS status = bootServices->AllocatePages(AllocateAnyPages,EfiLoaderData,lev1_page_count,&lev1_pages);
	if(status != EFI_SUCCESS)
		return status;

	bootServices->SetMem((void*)lev1_pages,MEMORY_PAGE_SIZE * lev1_page_count,0);

	wcprintf(L"lev1:%0x\r\n",lev1_page_count);

	UINT64 i;
	EFI_PHYSICAL_ADDRESS writeItr = lev2_page; 
	EFI_PHYSICAL_ADDRESS readItr = physicalBegin;
	EFI_PHYSICAL_ADDRESS pageItr = lev1_pages;
	for(i = 0;i < lev1_page_count;i++){
		//calc
		UINT64 lowerAlocateSize = ((allocateSize >> 9) & 0x1FF) ? 0x1FF : (allocateSize & 0x1FF);

		*((UINT64*)(VOID*)writeItr) = (flag & (~0x000FFFFFFFFFF000UL)) | ENTRY_PHYSICAL_ADDRESS(pageItr);
		status = allocateLev0PageTableLinear(pageItr,readItr,lowerAlocateSize,flag);
		if(status != EFI_SUCCESS)
			return status;

		writeItr += 8;
		readItr  += 0x200000;
		pageItr  += MEMORY_PAGE_SIZE;
		allocateSize -= 0x200;
	}

	return EFI_SUCCESS;
}

EFI_STATUS allocateLev0PageTableLinear(EFI_PHYSICAL_ADDRESS lev1_page,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag){
	UINTN lev0_page_count = ((allocateSize >> 0) & 0x1FF) + 1;

	UINT64 i;
	EFI_PHYSICAL_ADDRESS writeItr = lev1_page; 
	EFI_PHYSICAL_ADDRESS readItr = physicalBegin;
	for(i = 0;i < lev0_page_count;i++){
		*((UINT64*)(VOID*)writeItr) = (flag & (~0x000FFFFFFFFFF000UL)) | ENTRY_PHYSICAL_ADDRESS(readItr);

		writeItr += 8;
		readItr  += 0x1000;
		allocateSize -= 0x1;
	}

	return EFI_SUCCESS;
}