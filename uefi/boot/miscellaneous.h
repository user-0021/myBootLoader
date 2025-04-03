#pragma once
#include <uefi/uefi_defined.h>

typedef EFI_STATUS(EFIAPI *EFI_EXIT_BOOT_SERVICES)(EFI_HANDLE ImageHandle,UINTN MapKey);//すべてのブート サービスを終了します。
typedef EFI_STATUS(EFIAPI *EFI_SET_WATCHDOG_TIMER)(UINTN Timeout,UINT64 WatchdogCode,UINTN DataSize,CHAR16 *WatchdogData);//システムのウォッチドッグ タイマーを設定します。
typedef EFI_STATUS(EFIAPI *EFI_STALL)(UINTN Microseconds);//細粒度の失速を誘発します。
typedef VOID(EFIAPI *EFI_COPY_MEM)(VOID *Destination,VOID *Source,UINTN Length);//CopyMem ()関数は、あるバッファの内容を別のバッファにコピーします。
typedef VOID(EFIAPI *EFI_SET_MEM)(VOID *Buffer,UINTN Size,UINT8 Value);//SetMem()関数は、指定された値でバッファを埋めます。
typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_MONOTONIC_COUNT)(UINT64 *Count);//プラットフォームの単調に増加するカウントを返します。
typedef EFI_STATUS(EFIAPI *EFI_INSTALL_CONFIGURATION_TABLE)(EFI_GUID *Guid,VOID *Table);//EFI システム テーブルから構成テーブル エントリを追加、更新、または削除します。
typedef EFI_STATUS(EFIAPI *EFI_CALCULATE_CRC32)(VOID *Data,UINTN DataSize,UINT32 *Crc32);//データ バッファーの 32 ビット CRC を計算して返します。

