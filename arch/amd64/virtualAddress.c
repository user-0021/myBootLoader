#include <mem/virtualAddress.h>
#include <uefi/lib/consoleio.h>

#define SET_PRESENT(b) 					(((b) & 1UL) << 0)					//このページはメモリ内にある
#define SET_WRITABLE(b) 				(((b) & 1UL) << 1)					//このページへの書き込みは許可されている
#define SET_USER_ACCESSIBLE(b) 			(((b) & 1UL) << 2)					//0の場合、カーネルモードのみこのページにアクセスできる
#define SET_WRITE_THROUGH_CACHING(b) 	(((b) & 1UL) << 3)					//書き込みはメモリに対して直接行われる
#define SET_DISABLE_CACHE(b)			(((b) & 1UL) << 4)					//このページにキャッシュを使わない
#define SET_ACCESSED(b) 				(((b) & 1UL) << 5)					//このページが使われているとき、CPUはこのビットを1にする
#define SET_DIRTY(b) 					(((b) & 1UL) << 6)					//このページへの書き込みが行われたとき、CPUはこのビットを1にする
#define SET_HUGE_PAGE(b) 				(((b) & 1UL) << 7)					//P1とP4においては0で、P3においては1GiBのページを、P2においては2MiBのページを作る
#define SET_GROABAL(b) 					(((b) & 1UL) << 8)					//キャッシュにあるこのページはアドレス空間変更の際に初期化されない（CR4レジスタのPGEビットが1である必要がある）
#define SET_LAVAILEBLE(data)			(((data) & 7UL) << 9)				//OSが自由に使える
#define SET_PHYSICAL_ADDRESS(address)	(((address) & 0xFFFFFFFFFF000UL))	//ページ単位にアラインされた、フレームまたは次のページテーブルの52bit物理アドレス
#define SET_HAVAILEBLE(data)			(((data) & 0x7FUL) << 52)			//OSが自由に使える
#define SET_NO_EXECUTEBLE(b)			(((b) & 0x1UL) << 63)				//このページにおいてプログラムを実行することを禁じる（EFERレジスタのNXEビットが1である必要がある）

#define GET_PRESENT(v) 					(((v) >> 0) & 1UL)					//このページはメモリ内にある
#define GET_WRITABLE(v) 				(((v) >> 1) & 1UL)					//このページへの書き込みは許可されている
#define GET_USER_ACCESSIBLE(v) 			(((v) >> 2) & 1UL)					//0の場合、カーネルモードのみこのページにアクセスできる
#define GET_WRITE_THROUGH_CACHING(v) 	(((v) >> 3) & 1UL)					//書き込みはメモリに対して直接行われる
#define GET_DISABLE_CACHE(v)			(((v) >> 4) & 1UL)					//このページにキャッシュを使わない
#define GET_ACCESSED(v) 				(((v) >> 5) & 1UL)					//このページが使われているとき、CPUはこのビットを1にする
#define GET_DIRTY(v) 					(((v) >> 6) & 1UL)					//このページへの書き込みが行われたとき、CPUはこのビットを1にする
#define GET_HUGE_PAGE(v) 				(((v) >> 7) & 1UL)					//P1とP4においては0で、P3においては1GiBのページを、P2においては2MiBのページを作る
#define GET_GROABAL(v) 					(((v) >> 8) & 1UL)					//キャッシュにあるこのページはアドレス空間変更の際に初期化されない（CR4レジスタのPGEビットが1である必要がある）
#define GET_LAVAILEBLE(v)				(((v) >> 9) & 7UL)					//OSが自由に使える
#define GET_PHYSICAL_ADDRESS(v)			(( v      ) & 0xFFFFFFFFFF000UL)	//ページ単位にアラインされた、フレームまたは次のページテーブルの52bit物理アドレス
#define GET_HAVAILEBLE(v)				(((v) >> 52) & 0x7FUL)				//OSが自由に使える
#define GET_NO_EXECUTEBLE(v)			(((v) >> 63) & 0x1UL)				//このページにおいてプログラムを実行することを禁じる（EFERレジスタのNXEビットが1である必要がある）

#define VADDRESS_GET_LEV4_OFFSET(address)	(((address) >> 39) & 0x1FF)
#define VADDRESS_GET_LEV3_OFFSET(address)	(((address) >> 30) & 0x1FF)
#define VADDRESS_GET_LEV2_OFFSET(address)	(((address) >> 21) & 0x1FF)
#define VADDRESS_GET_LEV1_OFFSET(address)	(((address) >> 12) & 0x1FF)
#define VADDRESS_GET_PAGE_OFFSET(address)	(((address) >>  0) & 0xFFF)

#define ADDRESS_IS_ALIGNE_512GIB(address)	    (((address) & 0x7FFFFFFFFF) == 0)


//GDTPTR
typedef struct{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) GDT_PTR;


EFI_STATUS allocateLev3PageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev4_page,EFI_VIRTUAL_ADDRESS virtualBegin,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag);

EFI_STATUS createVirtualMap(EFI_BOOT_SERVICES* bootServices,EFI_LOADED_IMAGE_PROTOCOL* bootloader,BINARY_FILE* kernel,BOOT_LOADER_DATA* data){
	EFI_PHYSICAL_ADDRESS lev4_page;
	EFI_STATUS status;

	wcprintf(L"CreateVirtualMap..\r\n\n");

	//allocate lev4 page
	wcprintf(L"Try allocate lev4 table..\r\n");
	status = bootServices->AllocatePages(AllocateAnyPages,MEMORY_TYPE_PAGE,1,&lev4_page);
	CHECK_SUCCSESS(status);
	bootServices->SetMem((void*)lev4_page,MEMORY_PAGE_SIZE,0);
	
	//copy bootloader text
	UINT16 page3_count = 1 + VADDRESS_GET_LEV4_OFFSET(((EFI_PHYSICAL_ADDRESS)bootloader->ImageBase)) - VADDRESS_GET_LEV4_OFFSET(((EFI_PHYSICAL_ADDRESS)bootloader->ImageBase + bootloader->ImageSize));
	EFI_PHYSICAL_ADDRESS cr3;

	asm volatile (
		"mov %%cr3, %[cr3_reg]\n\t"
		:[cr3_reg]"=r"(cr3)
	);

	EFI_PHYSICAL_ADDRESS offset = (VADDRESS_GET_LEV4_OFFSET(((EFI_PHYSICAL_ADDRESS)bootloader->ImageBase)) << 3);
	for(; page3_count != 0 ; page3_count-- , offset += (1 << 3)){
		*((UINT64*)(void*) (lev4_page + offset)) = *((UINT64*)(void*) (cr3 + offset));
	}



	// kernel text 
	wcprintf(L"Try allocate kernel text table..\r\n");
	EFI_PHYSICAL_ADDRESS kernelCode;
	status = bootServices->AllocatePages(AllocateAnyPages,MEMORY_TYPE_PAGE,kernel->size >> 12,&kernelCode);
	CHECK_SUCCSESS(status);
	bootServices->SetMem((VOID*)kernelCode,kernel->size,0);
	status = allocateLev3PageTableLinear(bootServices,lev4_page,MEMORY_KERNEL_HEAD
		,(EFI_PHYSICAL_ADDRESS)kernelCode,kernel->size >> 12,SET_PRESENT(1) | SET_GROABAL(1) | SET_NO_EXECUTEBLE(1));
	CHECK_SUCCSESS(status);
	
	UINTN i;
	for(i = 0;i < kernel->sectionCount;i++){
		
		EFI_PHYSICAL_ADDRESS flag = 0;
		if(!(kernel->sections[i].flag & BINARY_SECTION_READONRY))
			flag |= SET_WRITABLE(1);
		if(!(kernel->sections[i].flag & BINARY_SECTION_EXECUTE))
			flag |= SET_NO_EXECUTEBLE(1);


		bootServices->CopyMem((VOID*)(kernelCode + kernel->sections[i].base),(VOID*)kernel->sections[i].imagePos,kernel->sections[i].imageSize);
		status = allocateLev3PageTableLinear(bootServices,lev4_page,MEMORY_KERNEL_HEAD + (EFI_PHYSICAL_ADDRESS)kernel->sections[i].base
			,(EFI_PHYSICAL_ADDRESS)kernelCode + kernel->sections[i].base,kernel->sections[i].sectionSize >> 12,SET_PRESENT(1) | SET_GROABAL(1));

		CHECK_SUCCSESS(status);
	}

	//make gdt table pointer
	wcprintf(L"Try allocate gdt table..\r\n");

	EFI_PHYSICAL_ADDRESS gdt;
	status = bootServices->AllocatePages(AllocateAnyPages,MEMORY_TYPE_PAGE,1,&gdt);
	CHECK_SUCCSESS(status);


	((long long *)gdt)[0] = 0x0000000000000000;//null
	((long long *)gdt)[1] = 0x00af9a000000ffff;//64bit code -kernel
	((long long *)gdt)[2] = 0x00cf93000000ffff;//64bit data -kernel
	((long long *)gdt)[3] = 0x00affa000000ffff;//64bit code -user
	((long long *)gdt)[4] = 0x00cff3000000ffff;//64bit data -user
	((long long *)gdt)[5] = 0x00cf9a000000ffff;//32bit code -kernel
	((long long *)gdt)[6] = 0x00cf93000000ffff;//32bit data -kernel

	*(GDT_PTR*)(void*)(gdt + 0x08 * 7) = (GDT_PTR){
		.limit = (0x08 * 7) - 1,
		.base = gdt
	};

	data->gdtptr = gdt + 0x08 * 7;
	data->page4 = lev4_page;
	data->kernelEntry = MEMORY_KERNEL_HEAD + MEMORY_PAGE_SIZE;

	return EFI_SUCCESS;
}

UINT64 countInvalidTable(EFI_PHYSICAL_ADDRESS parent,UINT64 offset,UINT64 count);

EFI_STATUS allocateLev2PageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev3_page,EFI_VIRTUAL_ADDRESS virtualBegin,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag);
EFI_STATUS allocateLev1PageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev2_page,EFI_VIRTUAL_ADDRESS virtualBegin,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag);
EFI_STATUS allocateLev0PageTableLinear(EFI_PHYSICAL_ADDRESS lev1_page,EFI_VIRTUAL_ADDRESS virtualBegin,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag);

EFI_STATUS allocateLev3PageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev4_page,EFI_VIRTUAL_ADDRESS virtualBegin,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag){
	
	EFI_STATUS status;
	EFI_PHYSICAL_ADDRESS lev3_pages;
	UINTN lev3_page_count = (allocateSize & 0x7FFFFFF) ? (((allocateSize >> 27) & 0x1FF) + 1) : ((allocateSize >> 27) & 0x1FF);
	UINTN need_allocate_size = countInvalidTable(lev4_page,VADDRESS_GET_LEV4_OFFSET(virtualBegin),lev3_page_count);

	if(need_allocate_size != 0){
		status = bootServices->AllocatePages(AllocateAnyPages,,need_allocate_size,&lev3_pages);
		if(status != EFI_SUCCESS)
			return status;
	
	
		bootServices->SetMem((void*)lev3_pages,MEMORY_PAGE_SIZE * need_allocate_size,0);
	}

	UINT64 i;
	EFI_PHYSICAL_ADDRESS writeItr = lev4_page + (VADDRESS_GET_LEV4_OFFSET(virtualBegin) << 3); 
	EFI_PHYSICAL_ADDRESS readItr = physicalBegin;
	EFI_PHYSICAL_ADDRESS pageItr = lev3_pages;
	for(i = 0;i < lev3_page_count;i++){

		//calc
		UINT64 lowerAlocateSize = ((allocateSize >> 27) & 0x1FF) ? 0x7FFFFFF : (allocateSize & 0x7FFFFFF);

		//get next page
		UINT64 page;
		if(GET_PRESENT(*((UINT64*)(VOID*)writeItr))){
			*((UINT64*)(VOID*)writeItr) |= (flag & (~0x800FFFFFFFFFF1D8UL));
			page = GET_PHYSICAL_ADDRESS(*((UINT64*)(VOID*)writeItr));
		}else{
			*((UINT64*)(VOID*)writeItr) = (flag & (~0x800FFFFFFFFFF1D8UL)) | SET_PHYSICAL_ADDRESS(pageItr);
			page = pageItr;
			pageItr  += MEMORY_PAGE_SIZE;
		}

		//calc offset
		if(i == 0)
			status = allocateLev2PageTableLinear(bootServices,page,virtualBegin,readItr,lowerAlocateSize,flag);
		else
			status = allocateLev2PageTableLinear(bootServices,page,virtualBegin & (~(0x1FFUL << 30)),readItr,lowerAlocateSize,flag);

		if(status != EFI_SUCCESS)
			return status;
		
		writeItr += 8;
		readItr  += 0x8000000000;
		allocateSize -= 0x8000000;
	}

	return EFI_SUCCESS;
}

EFI_STATUS allocateLev2PageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev3_page,EFI_VIRTUAL_ADDRESS virtualBegin,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag){

	EFI_STATUS status;
	EFI_PHYSICAL_ADDRESS lev2_pages;
	UINTN lev2_page_count = (allocateSize & 0x3FFFF) ? (((allocateSize >> 18) & 0x1FF) + 1) : ((allocateSize >> 18) & 0x1FF);
	UINTN need_allocate_size = countInvalidTable(lev3_page,VADDRESS_GET_LEV3_OFFSET(virtualBegin),lev2_page_count);

	if(need_allocate_size != 0){
		status = bootServices->AllocatePages(AllocateAnyPages,MEMORY_TYPE_PAGE,need_allocate_size,&lev2_pages);
		if(status != EFI_SUCCESS)
			return status;

		bootServices->SetMem((void*)lev2_pages,MEMORY_PAGE_SIZE * need_allocate_size,0);
	}

	UINT64 i;
	EFI_PHYSICAL_ADDRESS writeItr = lev3_page + (VADDRESS_GET_LEV3_OFFSET(virtualBegin) << 3); 
	EFI_PHYSICAL_ADDRESS readItr = physicalBegin;
	EFI_PHYSICAL_ADDRESS pageItr = lev2_pages;
	for(i = 0;i < lev2_page_count;i++){
		//calc
		UINT64 lowerAlocateSize = ((allocateSize >> 18) & 0x1FF) ? 0x3FFFF : (allocateSize & 0x3FFFF);

		//get next page
		UINT64 page;
		if(GET_PRESENT(*((UINT64*)(VOID*)writeItr))){
			*((UINT64*)(VOID*)writeItr) |= (flag & (~0x800FFFFFFFFFF158UL));
			page = GET_PHYSICAL_ADDRESS(*((UINT64*)(VOID*)writeItr));
		}else{
			*((UINT64*)(VOID*)writeItr) = (flag & (~0x800FFFFFFFFFF158UL)) | SET_PHYSICAL_ADDRESS(pageItr);
			page = pageItr;
			pageItr  += MEMORY_PAGE_SIZE;
		}
		
		if(i == 0)
			status = allocateLev1PageTableLinear(bootServices,page,virtualBegin,readItr,lowerAlocateSize,flag);
		else
			status = allocateLev1PageTableLinear(bootServices,page,virtualBegin & (~(0x1FF << 21)),readItr,lowerAlocateSize,flag);

		if(status != EFI_SUCCESS)
			return status;

		writeItr += 8;
		readItr  += 0x40000000;
		allocateSize -= 0x40000;
	}

	return EFI_SUCCESS;
}

EFI_STATUS allocateLev1PageTableLinear(EFI_BOOT_SERVICES* bootServices,EFI_PHYSICAL_ADDRESS lev2_page,EFI_VIRTUAL_ADDRESS virtualBegin,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag){

	EFI_STATUS status;
	EFI_PHYSICAL_ADDRESS lev1_pages;
	UINTN lev1_page_count = (allocateSize & 0x1FF) ? (((allocateSize >> 9) & 0x1FF) + 1) : ((allocateSize >> 9) & 0x1FF);
	UINTN need_allocate_size = countInvalidTable(lev2_page,VADDRESS_GET_LEV2_OFFSET(virtualBegin),lev1_page_count);

	if(need_allocate_size != 0){
		status = bootServices->AllocatePages(AllocateAnyPages,MEMORY_TYPE_PAGE,need_allocate_size,&lev1_pages);
		if(status != EFI_SUCCESS)
			return status;

		bootServices->SetMem((void*)lev1_pages,MEMORY_PAGE_SIZE * need_allocate_size,0);
	}

	UINT64 i;
	EFI_PHYSICAL_ADDRESS writeItr = lev2_page + (VADDRESS_GET_LEV2_OFFSET(virtualBegin) << 3); 
	EFI_PHYSICAL_ADDRESS readItr = physicalBegin;
	EFI_PHYSICAL_ADDRESS pageItr = lev1_pages;
	for(i = 0;i < lev1_page_count;i++){
		//calc
		UINT64 lowerAlocateSize = ((allocateSize >> 9) & 0x1FF) ? 0x1FF : (allocateSize & 0x1FF);

		//get next page
		UINT64 page;
		if(GET_PRESENT(*((UINT64*)(VOID*)writeItr))){
			*((UINT64*)(VOID*)writeItr) |= (flag & (~0x800FFFFFFFFFF158UL));
			page = GET_PHYSICAL_ADDRESS(*((UINT64*)(VOID*)writeItr)) ;
		}else{
			*((UINT64*)(VOID*)writeItr) = (flag & (~0x800FFFFFFFFFF158UL)) | SET_PHYSICAL_ADDRESS(pageItr);
			page = pageItr;
			pageItr  += MEMORY_PAGE_SIZE;
		}

		if(i == 0)
			status = allocateLev0PageTableLinear(page,virtualBegin,readItr,lowerAlocateSize,flag);
		else
			status = allocateLev0PageTableLinear(page,virtualBegin & (~(0x1FF << 12)),readItr,lowerAlocateSize,flag);
		
		if(status != EFI_SUCCESS)
			return status;

		writeItr += 8;
		readItr  += 0x200000;
		allocateSize -= 0x200;
	}

	return EFI_SUCCESS;
}

EFI_STATUS allocateLev0PageTableLinear(EFI_PHYSICAL_ADDRESS lev1_page,EFI_VIRTUAL_ADDRESS virtualBegin,EFI_PHYSICAL_ADDRESS physicalBegin,UINT64 allocateSize,EFI_PHYSICAL_ADDRESS flag){
	UINTN lev0_page_count = ((allocateSize >> 0) & 0x1FF);
	
	UINT64 i;
	EFI_PHYSICAL_ADDRESS writeItr = lev1_page + (VADDRESS_GET_LEV1_OFFSET(virtualBegin) << 3); 
	EFI_PHYSICAL_ADDRESS readItr = physicalBegin;
	for(i = 0;i < lev0_page_count;i++){
		*((UINT64*)(VOID*)writeItr) = (flag & (~0x000FFFFFFFFFF000UL)) | SET_PHYSICAL_ADDRESS(readItr);

		writeItr += 8;
		readItr  += 0x1000;
		allocateSize -= 0x1;
	}

	return EFI_SUCCESS;
}


UINT64 countInvalidTable(EFI_PHYSICAL_ADDRESS parent,UINT64 offset,UINT64 count){
	
	if((offset + count) > 512 || (offset) > 512 || (count) > 512){
		return 0;
	}

	UINT64 counter = 0;
	EFI_PHYSICAL_ADDRESS itr = parent + (offset << 3);
	
	UINT64 i;
	for(i = 0;i < count;i++){

		if(GET_PRESENT(*(UINT64*)(void*)itr) == 0){
			counter++;
		}

		itr += 8;
	}

	return counter;
}