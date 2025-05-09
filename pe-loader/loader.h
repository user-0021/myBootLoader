#pragma once
#include <uefi/uefi_defined.h>

EFI_STATUS load_pe(void* peFile,UINTN fileSize,void* mem,UINTN* memPages);

