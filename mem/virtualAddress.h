#pragma once
#include <pe-loader/loader.h>

#define MEMORY_PAGE_SIZE 0x1000UL
#define MEMORY_PAGE_MASK (-1UL + MEMORY_PAGE_SIZE)
#define MEMORY_DIRECTMAP_HEAD 0xffff880000000000UL
#define MEMORY_KERNEL_HEAD    0xffffffff80000000UL

#define MEMORY_TYPE_PAGE 0x80000001


#ifndef __ASSEMBLER__
#include <uefi/uefi.h>
#include <uefi/protocol/loaded_image.h>
#include <init/init.h>


EFI_STATUS createVirtualMap(EFI_BOOT_SERVICES* bootService,EFI_LOADED_IMAGE_PROTOCOL* bootloader,BINARY_FILE* kernel,BOOT_LOADER_DATA* data);

#endif
