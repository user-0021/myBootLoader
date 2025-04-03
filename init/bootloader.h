#pragma once
#include <uefi/uefi.h>
#include <uefi/protocol/loaded_image.h>

typedef struct
{
	EFI_DEVICE_PATH_PROTOCOL* devPath;
	EFI_LOADED_IMAGE_PROTOCOL* loadedImage;
	EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* devPathToText;
	EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL* devPathFromText;
} PROTOCOL_LIST;

typedef struct
{
	PROTOCOL_LIST protocols;
} BOOTLOADER_DATA;


EFI_STATUS init_protocol(EFI_HANDLE imageHandle,EFI_SYSTEM_TABLE* system,PROTOCOL_LIST* list);
EFI_STATUS init_bootloader(EFI_SYSTEM_TABLE *SystemTable,BOOTLOADER_DATA* data);