#pragma once
#include <uefi/uefi_defined.h>

typedef struct _memory_block
{
	EFI_PHYSICAL_ADDRESS head;
	UINTN pageSize;
	struct _memory_block* next;
} memory_block;
