#pragma once
#include <uefi/uefi.h>
#include <uefi/protocol/loaded_image.h>
#include <uefi/console/graphic.h>

typedef struct
{
	EFI_DEVICE_PATH_PROTOCOL* devPath;
	EFI_LOADED_IMAGE_PROTOCOL* bootLoaderImage;
	EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* devPathToText;
	EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL* devPathFromText;
	EFI_DEVICE_PATH_UTILITIES_PROTOCOL* devPathUtilities;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* graphicsOut;
} PROTOCOL_LIST;

typedef struct
{
	VOID* acpi20Table;
	VOID* acpiTable;
	VOID* smBios3Table;
	VOID* smBiosTable;
} TABLE_LIST;

typedef struct
{
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION Info;
} DATA_LIST;

typedef struct
{
	PROTOCOL_LIST protocols;
	TABLE_LIST tables;
	DATA_LIST data;
} BOOTLOADER_DATA;


EFI_STATUS init_protocol(CONST EFI_HANDLE imageHandle,CONST EFI_SYSTEM_TABLE* system,PROTOCOL_LIST* list);
EFI_STATUS init_bootloader(EFI_SYSTEM_TABLE *SystemTable,BOOTLOADER_DATA* data,UINTN* mmapKey);