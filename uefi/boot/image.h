#pragma once
#include <uefi/uefi_defined.h>
#include <uefi/uefi.h>
#include <uefi/boot/protocol.h>

typedef EFI_STATUS(EFIAPI *EFI_IMAGE_LOAD)(BOOLEAN BootPolicy,EFI_HANDLE ParentImageHandle,EFI_DEVICE_PATH_PROTOCOL *DevicePath,VOID *SourceBuffer,UINTN SourceSize,EFI_HANDLE *ImageHandle);//EFI イメージをメモリにロードします。
typedef EFI_STATUS(EFIAPI *EFI_IMAGE_START)(EFI_HANDLE ImageHandle,UINTN *ExitDataSize,CHAR16 **ExitData);//読み込まれたイメージのエントリ ポイントに制御を転送します。
typedef EFI_STATUS(EFIAPI *EFI_IMAGE_UNLOAD)(EFI_HANDLE ImageHandle);//イメージをアンロードします。
typedef EFI_STATUS(EFIAPI *EFI_IMAGE_ENTRY_POINT)(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable);//これは、EFI イメージ エントリ ポイントの宣言です。これは、この仕様に従って記述されたアプリケーション、EFI ブート サービス ドライバー、または EFI ランタイム ドライバーへのエントリ ポイントになります。
typedef EFI_STATUS(EFIAPI *EFI_EXIT)(EFI_HANDLE ImageHandle,EFI_STATUS ExitStatus,UINTN ExitDataSize,CHAR16 *ExitData);//ロードされた EFI イメージを終了し、ブート サービスに制御を返します。
	 