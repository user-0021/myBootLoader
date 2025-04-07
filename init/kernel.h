#pragma once
#include <init/bootloader.h>
#include <uefi/uefi_defined.h>
#include <uefi/console/console.h>

EFI_STATUS load_kernel(CONST EFI_SYSTEM_TABLE* systemTable,CONST EFI_HANDLE parent,CONST PROTOCOL_LIST* list,CONST CHAR16* kernel_file); 
