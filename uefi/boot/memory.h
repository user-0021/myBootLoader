#pragma once
#include <uefi/uefi_defined.h>

//******************************************************
// Memory Attribute Definitions
//******************************************************
// These types can be "ORed" together as needed.
#define EFI_MEMORY_UC              0x0000000000000001
#define EFI_MEMORY_WC              0x0000000000000002
#define EFI_MEMORY_WT              0x0000000000000004
#define EFI_MEMORY_WB              0x0000000000000008
#define EFI_MEMORY_UCE             0x0000000000000010
#define EFI_MEMORY_WP              0x0000000000001000
#define EFI_MEMORY_RP              0x0000000000002000
#define EFI_MEMORY_XP              0x0000000000004000
#define EFI_MEMORY_NV              0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE   0x0000000000010000
#define EFI_MEMORY_RO              0x0000000000020000
#define EFI_MEMORY_SP              0x0000000000040000
#define EFI_MEMORY_CPU_CRYPTO      0x0000000000080000
#define EFI_MEMORY_RUNTIME         0x8000000000000000
#define EFI_MEMORY_ISA_VALID       0x4000000000000000
#define EFI_MEMORY_ISA_MASK        0x0FFFF00000000000


//******************************************************
//EFI_ALLOCATE_TYPE
//******************************************************
// These types are discussed in the "Description" section below.
typedef enum {
   AllocateAnyPages,
   AllocateMaxAddress,
   AllocateAddress,
   MaxAllocateType
} EFI_ALLOCATE_TYPE;

//******************************************************
//EFI_MEMORY_TYPE
//******************************************************
// These type values are discussed in Memory Type Usage before ExitBootServices()  and  Memory Type Usage after ExitBootServices().
typedef enum {
   EfiReservedMemoryType,
   EfiLoaderCode,
   EfiLoaderData,
   EfiBootServicesCode,
   EfiBootServicesData,
   EfiRuntimeServicesCode,
   EfiRuntimeServicesData,
   EfiConventionalMemory,
   EfiUnusableMemory,
   EfiACPIReclaimMemory,
   EfiACPIMemoryNVS,
   EfiMemoryMappedIO,
   EfiMemoryMappedIOPortSpace,
   EfiPalCode,
   EfiPersistentMemory,
   EfiUnacceptedMemoryType,
   EfiMaxMemoryType
} EFI_MEMORY_TYPE;


typedef struct _EFI_MEMORY_DESCRIPTOR EFI_MEMORY_DESCRIPTOR;//仮想メモリの情報


typedef EFI_STATUS(EFIAPI *EFI_ALLOCATE_PAGES)(EFI_ALLOCATE_TYPE Type,EFI_MEMORY_TYPE MemoryType,UINTN Pages,EFI_PHYSICAL_ADDRESS *Memory);//システムからメモリ ページを割り当てます。
typedef EFI_STATUS(EFIAPI *EFI_FREE_PAGES)(EFI_PHYSICAL_ADDRESS Memory,UINTN Pages);//メモリ ページを解放します。
typedef EFI_STATUS(EFIAPI *EFI_GET_MEMORY_MAP)(UINTN *MemoryMapSize,EFI_MEMORY_DESCRIPTOR *MemoryMap,UINTN *MapKey,UINTN *DescriptorSize,UINT32 *DescriptorVersion);//現在のメモリ マップを返します。
typedef EFI_STATUS(EFIAPI  *EFI_ALLOCATE_POOL)(EFI_MEMORY_TYPE PoolType,UINTN Size,VOID **Buffer);//プールメモリを割り当てます。
typedef EFI_STATUS(EFIAPI *EFI_FREE_POOL)(VOID *Buffer);//プール メモリをシステムに返します。


typedef struct _EFI_MEMORY_DESCRIPTOR{
	UINT32                     Type;
	EFI_PHYSICAL_ADDRESS       PhysicalStart;
	EFI_VIRTUAL_ADDRESS        VirtualStart;
	UINT64                     NumberOfPages;
	UINT64                     Attribute;
} EFI_MEMORY_DESCRIPTOR;