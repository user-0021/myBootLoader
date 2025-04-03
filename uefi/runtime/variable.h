#pragma once
#include <uefi/uefi_defined.h>

//******************************************************
// Variable Attributes
//******************************************************
#define EFI_VARIABLE_NON_VOLATILE                           0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS                     0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS                         0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD                  0x00000008 
//This attribute is identified by the mnemonic 'HR' elsewhere
//in this specification.
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS             0x00000010
//NOTE: EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS is deprecated
//and should be considered reserved.
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS  0x00000020
#define EFI_VARIABLE_APPEND_WRITE                           0x00000040
#define EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS          0x00000080


typedef EFI_STATUS(EFIAPI *EFI_GET_VARIABLE)(CHAR16 *VariableName,EFI_GUID *VendorGuid,UINT32 *Attributes,UINTN *DataSize,VOID *Data);//変数の値を返します。
typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_VARIABLE_NAME)(UINTN *VariableNameSize,CHAR16 *VariableName,EFI_GUID *VendorGuid);//現在の変数名を列挙します。
typedef EFI_STATUS(EFIAPI *EFI_SET_VARIABLE)(CHAR16 *VariableName,EFI_GUID *VendorGuid,UINT32 Attributes,UINTN DataSize,VOID *Data);//変数の値を設定します。このサービスを使用して、新しい変数を作成したり、既存の変数の値を変更したり、既存の変数を削除したりできます。
typedef EFI_STATUS(EFIAPI *EFI_QUERY_VARIABLE_INFO)(UINT32 Attributes,UINT64 *MaximumVariableStorageSize,UINT64 *RemainingVariableStorageSize,UINT64 *MaximumVariableSize);//EFI 変数に関する情報を返します。


