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
#define ENTRY_PHYSICAL_ADDRESS(address)	(((address) & 0xFFFFFFFFFFUL) << 12)	//ページ単位にアラインされた、フレームまたは次のページテーブルの52bit物理アドレス
#define ENTRY_HAVAILEBLE(data)			(((data) & 0x7FUL) << 52)				//OSが自由に使える
#define ENTRY_NO_EXECUTEBLE(b)			(((b) & 0x1UL) << 63)					//このページにおいてプログラムを実行することを禁じる（EFERレジスタのNXEビットが1である必要がある）

#define VADDRESS_GET_LEV4_OFFSET(address)	(((address) >> 39) & 0x1FF)
#define VADDRESS_GET_LEV3_OFFSET(address)	(((address) >> 30) & 0x1FF)
#define VADDRESS_GET_LEV2_OFFSET(address)	(((address) >> 21) & 0x1FF)
#define VADDRESS_GET_LEV1_OFFSET(address)	(((address) >> 12) & 0x1FF)
#define VADDRESS_GET_PAGE_OFFSET(address)	(((address) >>  0) & 0xFFF)

EFI_PHYSICAL_ADDRESS createVirtualMap(EFI_BOOT_SERVICES* bootServices,EFI_LOADED_IMAGE_PROTOCOL* kernel){
	EFI_PHYSICAL_ADDRESS memoryEnd;
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

			memoryEnd = ((EFI_MEMORY_DESCRIPTOR*)desc)->PhysicalStart + (((EFI_MEMORY_DESCRIPTOR*)desc)->NumberOfPages << 12);

		wcprintf(L"...Success\r\n");
		wcprintf(L"Memory availeble length:%0x\r\n",memoryEnd);
	}else{
		wcprintf(L"...Failed(ErrorCode:%x)\r\n\n",status);
		return (EFI_PHYSICAL_ADDRESS)NULL;
	}


	// memory direct map 
	UINT64 i;
	UINT64 limit;
	i = VADDRESS_GET_LEV4_OFFSET(MEMORY_DIRECTMAP_HEAD);
	EFI_PHYSICAL_ADDRESS directMap_lev3_pages;
	EFI_PHYSICAL_ADDRESS directMapEnd = memoryEnd + MEMORY_DIRECTMAP_HEAD;
	UINT64 directMapSize = (VADDRESS_GET_LEV4_OFFSET(directMapEnd) - i) + 1;

	wcprintf(L"Try allocate direct map table..\r\n");
	status = bootServices->AllocatePages(AllocateAnyPages,EfiLoaderData,directMapSize,&directMap_lev3_pages);
	if(status != EFI_SUCCESS){
		wcprintf(L"...Failed(ErrorCode:%x)\r\n\n",status);
		return (EFI_PHYSICAL_ADDRESS)NULL;
	}else{
		wcprintf(L"...Success\r\n");
		wcprintf(L"Locate on %0x\r\n\n",(UINT64)directMap_lev3_pages);
	}

	bootServices->SetMem((void*)directMap_lev3_pages,MEMORY_PAGE_SIZE * directMapSize,0);
	limit = VADDRESS_GET_LEV4_OFFSET(directMapEnd);
	for(; i <= limit ;i++){
		((UINT64*)lev4_page)[i] = ENTRY_PRESENT(1) | ENTRY_WRITABLE(1) | ENTRY_GROABAL(1) 
			| ENTRY_NO_EXECUTEBLE(1) | ENTRY_PHYSICAL_ADDRESS(directMap_lev3_pages);
		directMap_lev3_pages += MEMORY_PAGE_SIZE;
	}

	return lev4_page;
}