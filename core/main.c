#include <uefi/uefi.h>
#include <uefi/lib/consoleio.h>
#include <init/bootloader.h>
#include <mem/virtualAddress.h>

#include "config.h"

#define KERNEL_PATH L"kernel/kernel.efi"

extern VOID jmp_kernel(BOOT_LOADER_DATA* data,KERNEL_INFO* info);

inline VOID pain(KERNEL_INFO* info){
	VOID *FrameBufferBase = (void*)info->data.screenBuffer;
	UINT32 Width = info->data.gInfo.HorizontalResolution;
	UINT32 Height = info->data.gInfo.VerticalResolution;
	UINT32 PixelsPerScanLine = info->data.gInfo.PixelsPerScanLine;

	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *FB = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)FrameBufferBase;

	// 画面を真っ赤に塗る
	for (UINTN y = 0; y < Height; y++) {
		for (UINTN x = 0; x < Width; x++) {
			FB[y * PixelsPerScanLine + x].Red = 0;
			FB[y * PixelsPerScanLine + x].Green = 255;
			FB[y * PixelsPerScanLine + x].Blue = 0;
			FB[y * PixelsPerScanLine + x].Reserved = 0;
		}
	}
}


EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable){
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	stdout = SystemTable->ConOut;

	wcprintf(L"Success lunch bootloader\r\n\n");
	wcprintf(L"Load firmware info\r\n");
	wcprintf(L"FirmwareVender   : %s\r\n",SystemTable->FirmwareVendor);
	wcprintf(L"FirmwareRevision : 0x%0x\r\n\n",(UINT64)SystemTable->FirmwareRevision);

	//init data
	BOOT_LOADER_DATA data = {0};
	KERNEL_INFO info = {0};

	//init protpcol
	if(init_protocol(SystemTable,ImageHandle,&data) != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*3);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	//load kernel
	if(init_kernel(SystemTable,ImageHandle,&data,KERNEL_PATH) != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*200);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}


	//get Graphic mode
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *gInfo;
	UINTN sizeOfInfo;
	if(data.protocols.graphicsOut->QueryMode(data.protocols.graphicsOut, data.protocols.graphicsOut->Mode->Mode, &sizeOfInfo, &gInfo) != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*200);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}
	info.data.gInfo = *gInfo;
	info.data.screenBuffer = data.protocols.graphicsOut->Mode->FrameBufferBase;
	
	//init bootloader
	if(init_bootloader(SystemTable,ImageHandle,&info) != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*3);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	//jmp kernel
	jmp_kernel(&data,&info);

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

	//多分呼ばれない
	return EFI_SUCCESS;
}