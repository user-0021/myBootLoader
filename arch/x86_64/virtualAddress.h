#pragma once
#include <uefi/boot/boot.h>
#include <uefi/protocol/loaded_image.h>

EFI_PHYSICAL_ADDRESS* createVirtualMap(EFI_BOOT_SERVICES* bootService,EFI_LOADED_IMAGE_PROTOCOL* kernel,EFI_VIRTUAL_ADDRESS* preaceAddress);