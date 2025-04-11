#include <uefi/uefi.h>
#include <uefi/lib/consoleio.h>
#include <init/bootloader.h>
#include <init/kernel.h>
#include <mem/virtualAddress.h>

#include "config.h"

#define KERNEL_PATH L"kernel/kernel.efi"


EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable){
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	s_wcprintf(SystemTable->ConOut,L"Success lunch bootloader\r\n\n");
	s_wcprintf(SystemTable->ConOut,L"Load firmware info\r\n");
	s_wcprintf(SystemTable->ConOut,L"FirmwareVender   : %s\r\n",SystemTable->FirmwareVendor);
	s_wcprintf(SystemTable->ConOut,L"FirmwareRevision : 0x%0x\r\n\n",(UINT64)SystemTable->FirmwareRevision);


	BOOTLOADER_DATA data = {0};

	//init protpcol
	if(init_protocol(ImageHandle,SystemTable,&data.protocols) != EFI_SUCCESS){
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	//load kernel
	if(load_kernel(SystemTable,ImageHandle,&data.protocols,KERNEL_PATH) != EFI_SUCCESS){
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	void* pointer = createVirtualMap(SystemTable->BootServices,NULL,NULL);
	int i;
	for(i = 0;i < 512;i++){
		s_wcprintf(SystemTable->ConOut,L"%0x\r\n",((UINT64*)pointer)[i]);
	}

	//init bootloader
	if(init_bootloader(SystemTable,&data) != EFI_SUCCESS){
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	SystemTable->BootServices->Stall(5000000);


	//多分呼ばれない
	// SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	return EFI_SUCCESS;
}