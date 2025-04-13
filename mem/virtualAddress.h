#pragma once
#include <uefi/uefi.h>
#include <uefi/protocol/loaded_image.h>

#define MEMORY_PAGE_SIZE 0x1000
#define MEMORY_DIRECTMAP_HEAD 0xffff880000000000
#define MEMORY_KERNEL_HEAD    0xffffffff80000000

EFI_PHYSICAL_ADDRESS createVirtualMap(EFI_BOOT_SERVICES* bootService,EFI_LOADED_IMAGE_PROTOCOL* kernel);