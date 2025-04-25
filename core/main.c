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

extern EFI_PHYSICAL_ADDRESS jmp_kernel(EFI_PHYSICAL_ADDRESS gdtptr,EFI_PHYSICAL_ADDRESS page4,BOOTLOADER_DATA* data);


EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable){
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	stdout = SystemTable->ConOut;

	wcprintf(L"Success lunch bootloader%0x\r\n\n");
	wcprintf(L"Load firmware info\r\n");
	wcprintf(L"FirmwareVender   : %s\r\n",SystemTable->FirmwareVendor);
	wcprintf(L"FirmwareRevision : 0x%0x\r\n\n",(UINT64)SystemTable->FirmwareRevision);


	//make gdt table pointer
	EFI_PHYSICAL_ADDRESS gdt;
	EFI_STATUS status = SystemTable->BootServices->AllocatePages(AllocateAnyPages,EfiLoaderData,1,&gdt);
	if(status != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*10);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

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
		SystemTable->BootServices->Stall(1000*1000*3);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	//load kernel
	EFI_PHYSICAL_ADDRESS kernel_page4;
	if(load_kernel(SystemTable,ImageHandle,&data.protocols,KERNEL_PATH,&kernel_page4) != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*200);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	//init bootloader
	UINTN mapKey = 0;
	if(init_bootloader(SystemTable,&data,&mapKey) != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*3);
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


	// UINT64 cr0,cr2,cr3,cr4;
	// asm volatile ("mov %%cr0, %[reg]\n\t" : [reg]"=r"(cr0));
	// asm volatile ("mov %%cr2, %[reg]\n\t" : [reg]"=r"(cr2));
	// asm volatile ("mov %%cr3, %[reg]\n\t" : [reg]"=r"(cr3));
	// asm volatile ("mov %%cr4, %[reg]\n\t" : [reg]"=r"(cr4));

	// wcprintf(L"cr0: %0x\r\n",cr0);
	// wcprintf(L"cr2: %0x\r\n",cr2);
	// wcprintf(L"cr3: %0x\r\n",cr3);
	// wcprintf(L"cr4: %0x\r\n",cr4);

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
	UINTN SizeOfInfo;
	status = data.protocols.graphicsOut->QueryMode(data.protocols.graphicsOut, data.protocols.graphicsOut->Mode->Mode, &SizeOfInfo, &Info);
	data.data.Info = *Info;

	//Exit Boot Service
	status = SystemTable->BootServices->ExitBootServices(ImageHandle,mapKey);
	if(status != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*3);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	
	//RCX and RDX
	jmp_kernel((EFI_PHYSICAL_ADDRESS)&gdtr,kernel_page4,&data);

	//多分呼ばれない
	return EFI_SUCCESS;
}