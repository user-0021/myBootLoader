#include <uefi/uefi.h>
#include <uefi/lib/consoleio.h>
#include <init/bootloader.h>
#include <init/kernel.h>
#include <mem/virtualAddress.h>

#include "config.h"

#define KERNEL_PATH L"kernel/kernel.efi"


typedef struct{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) GDTPtr;



extern EFI_PHYSICAL_ADDRESS jmp_kernel(EFI_PHYSICAL_ADDRESS gdtptr,EFI_PHYSICAL_ADDRESS page4);



EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable){
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	stdout = SystemTable->ConOut;

	wcprintf(L"Success lunch bootloader\r\n\n");
	wcprintf(L"Load firmware info\r\n");
	wcprintf(L"FirmwareVender   : %s\r\n",SystemTable->FirmwareVendor);
	wcprintf(L"FirmwareRevision : 0x%0x\r\n\n",(UINT64)SystemTable->FirmwareRevision);


	//make gdt table pointer
	EFI_PHYSICAL_ADDRESS gdt;
	EFI_STATUS status = SystemTable->BootServices->AllocatePages(AllocateAnyPages,EfiLoaderData,1,&gdt);
	if(status != EFI_SUCCESS)
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);

	((long long *)gdt)[0] = 0x0000000000000000;
	((long long *)gdt)[1] = 0x00af9a000000ffff;
	((long long *)gdt)[2] = 0x00cf93000000ffff;

	GDTPtr gdtr = {
		.limit = (0x08 * 3) - 1,
		.base = gdt
	};

	BOOTLOADER_DATA data = {0};

	//init protpcol
	if(init_protocol(ImageHandle,SystemTable,&data.protocols) != EFI_SUCCESS){
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	//load kernel
	EFI_PHYSICAL_ADDRESS kernel_page4;
	if(load_kernel(SystemTable,ImageHandle,&data.protocols,KERNEL_PATH,&kernel_page4) != EFI_SUCCESS){
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	//init bootloader
	if(init_bootloader(SystemTable,ImageHandle,&data) != EFI_SUCCESS){
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}
	
	/**********************
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 *   TOOOOOOOOOOOOOOOOOOOOOOOOOOOOODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
	 * 
	 *  実行ファイルの読み込みを自作する！ページのフラグ管理のため
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 */


	//RCX and RDX
	jmp_kernel((EFI_PHYSICAL_ADDRESS)&gdtr,kernel_page4);



	UINT64 cr3_flag = 0 & 0xFFF;
	EFI_PHYSICAL_ADDRESS kernel_start = MEMORY_KERNEL_HEAD + 0x1000;

	wcprintf(L"KERNEL:%0x\r\nPAGE4:%0x\r\nGDTP:%0x\r\nOFF:%0x\r\n",kernel_start,kernel_page4,gdt,MEMORY_DIRECTMAP_HEAD);
	asm volatile(

		//update memory map
		"mov %[kernel_page4], %%rax\n\t"
		"or %[cr3_flag], %%rax\n\t"
  		"mov %%rax, %%cr3\n\t"

		//Now reload the segment registers (CS, DS, SS, etc.) with the appropriate segment selectors...
		"lgdt (%[gdtr])\n\t"
		"mov $0x10, %%ax\n\t"
		"mov %%ax, %%ds\n\t"
		"mov %%ax, %%es\n\t"
		"mov %%ax, %%ss\n\t"
		"mov %%ax, %%fs\n\t"
		"mov %%ax, %%gs\n\t"

		"add %[memory_offset], %%rsp\n\t"
		
		//Reload CS with a 64-bit code selector by performing a long jmp
		"pushq $0x08\n\t"
		"pushq %[kernel_start]\n\t"
		"lretq\n\t"
		:
		:	
			[cr3_flag]"r"(cr3_flag),
			[gdtr]"r"((&gdtr)),
			[kernel_page4]"r"(kernel_page4),
			[kernel_start]"r"(kernel_start),
			[memory_offset]"r"(MEMORY_DIRECTMAP_HEAD)
		: 
	); 

	
	//多分呼ばれない
	// SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	return EFI_SUCCESS;
}