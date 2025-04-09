#pragma once
#include <uefi/boot/memory.h>
#include <uefi/protocol/loaded_image.h>

EFI_PHYSICAL_ADDRESS* createVirtualMap(EFI_ALLOCATE_PAGES allocatePage,EFI_LOADED_IMAGE_PROTOCOL* kernel,EFI_VIRTUAL_ADDRESS* preaceAddress);