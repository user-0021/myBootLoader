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
	if(init_bootloader(SystemTable,&data) != EFI_SUCCESS){
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	UINT64 msr_flag = (1 << 8) | (1 << 11);
	UINT64 cr0_flag = (1 << 31) | (1 << 11);
	UINT64 cr4_flag = (1 << 5);
	EFI_PHYSICAL_ADDRESS kernel_start = MEMORY_KERNEL_HEAD + 0x1000;
	asm volatile(
		//Enable PAE
		"mov %%cr4, %%rdx\n\t"
		"or  %%rdx, %[cr4_flag]\n\t"
		"mov %%rdx, %%cr4\n\t"
	
		//Set LME (long mode enable) 
		//Set NXE (Execute Disable bit enable)
		"mov $0xC0000080, %%rcx\n\t"
		"rdmsr\n\t"
		"or %%rax, %[msr_flag]\n\t"
		"wrmsr\n\t"
		
		"mov %%rax, %[kernel_page4]\n\t"
  		"mov %%cr3, %%rax\n\t"
	
		//Enable paging (and protected mode, if it isn't already active)
		"mov %%cr0, %%rbx\n\t"
		"or %%rbx, %[cr0_flag]\n\t"
		"mov %%rbx, %%cr0\n\t"

		//Now reload the segment registers (CS, DS, SS, etc.) with the appropriate segment selectors...
		"lgdt (%[gdtr])\n\t"
		"mov $0x10, %%ax\n\t"
		"mov %%ax, %%ds\n\t"
		"mov %%ax, %%es\n\t"
		"mov %%ax, %%ss\n\t"
		"mov %%ax, %%fs\n\t"
		"mov %%ax, %%gs\n\t"

		
		//Reload CS with a 64-bit code selector by performing a long jmp

		"pushq $0x08\n\t"
		"pushq %[kernel_start]\n\t"
		"lretq\n\t"
		:	
		:	[msr_flag]"r"(msr_flag),
			[cr0_flag]"r"(cr0_flag),
			[cr4_flag]"r"(cr4_flag),
			[gdtr]"r"((&gdtr)),
			[kernel_page4]"r"(kernel_page4),
			[kernel_start]"r"(kernel_start),
			[memory_offset]"r"(MEMORY_DIRECTMAP_HEAD)
	); 
	
	//多分呼ばれない
	// SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	return EFI_SUCCESS;
}