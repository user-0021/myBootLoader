#pragma once
#include <uefi/uefi.h>
#include <uefi/protocol/loaded_image.h>
#include <uefi/protocol/media_access.h>
#include <uefi/console/graphic.h>

#define INIT_CPU_IMAGE_PATH L"bootLoader\\init_cpu.efi"


typedef struct
{
	EFI_DEVICE_PATH_PROTOCOL* devPath;
	EFI_LOADED_IMAGE_PROTOCOL* bootLoaderImage;
	EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* devPathToText;
	EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL* devPathFromText;
	EFI_DEVICE_PATH_UTILITIES_PROTOCOL* devPathUtilities;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* simpleFileSystem;
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
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION gInfo;
	EFI_PHYSICAL_ADDRESS screenBuffer;
} DATA_LIST;

typedef struct
{
	TABLE_LIST tables;
	DATA_LIST data;
} KERNEL_INFO;

#ifdef __x86_64__
typedef struct
{
	PROTOCOL_LIST protocols;
	EFI_PHYSICAL_ADDRESS gdtptr;
	EFI_PHYSICAL_ADDRESS page4;
	EFI_PHYSICAL_ADDRESS kernelEntry;
	EFI_FILE_PROTOCOL* root;
} BOOT_LOADER_DATA;
#endif

EFI_STATUS init_bootloader(CONST EFI_SYSTEM_TABLE *SystemTable,CONST EFI_HANDLE imageHandle,KERNEL_INFO* info);
EFI_STATUS init_protocol  (CONST EFI_SYSTEM_TABLE* systemTable,CONST EFI_HANDLE imageHandle,BOOT_LOADER_DATA* data);
EFI_STATUS load_kernel    (CONST EFI_SYSTEM_TABLE* systemTable,CONST EFI_HANDLE imageHandle,BOOT_LOADER_DATA* data,CHAR16* kernel_file); 