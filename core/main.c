#include <uefi/uefi.h>
#include <uefi/lib/consoleio.h>
#include <init/bootloader.h>

#include "config.h"



EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable)
{
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	s_wcprintf(SystemTable->ConOut,L"Success lunch bootloader\r\n\n");
	s_wcprintf(SystemTable->ConOut,L"Load firmware info\r\n");
	s_wcprintf(SystemTable->ConOut,L"FirmwareVender   : %s\r\n",SystemTable->FirmwareVendor);
	s_wcprintf(SystemTable->ConOut,L"FirmwareRevision : 0x%x\r\n\n",(UINT64)SystemTable->FirmwareRevision);

	BOOTLOADER_DATA data;

	//init 
	if(init_protocol(ImageHandle,SystemTable,&data.protocols) != EFI_SUCCESS){
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	
	s_wcprintf(SystemTable->ConOut,L"Device path   : %s\r\n",data.protocols.devPathToText->ConvertDevicePathToText(data.protocols.devPath,FALSE,FALSE));

	// init_bootloader(SystemTable);

	SystemTable->BootServices->Stall(5000000);

	return EFI_SUCCESS;
}