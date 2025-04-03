#pragma once
#include <uefi/uefi_defined.h>
#include <uefi/boot/memory.h>

//******************************************************
// EFI_OPTIONAL_PTR
//******************************************************
#define EFI_OPTIONAL_PTR          0x00000001

typedef EFI_STATUS(EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP)(UINTN MemoryMapSize,UINTN DescriptorSize,UINT32 DescriptorVersion,EFI_MEMORY_DESCRIPTOR *VirtualMap);//EFI ファームウェアのランタイム アドレス指定モードを物理から仮想に変更します。
typedef EFI_STATUS(EFIAPI *EFI_CONVERT_POINTER)(UINTN DebugDisposition,VOID **Address);//後続のメモリ アクセスで使用される新しい仮想アドレスを決定します。
