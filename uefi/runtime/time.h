#pragma once
#include <uefi/uefi_defined.h>


//***************************************************
// Bit Definitions for EFI_TIME.Daylight. See below.
//***************************************************
#define EFI_TIME_ADJUST_DAYLIGHT   0x01
#define EFI_TIME_IN_DAYLIGHT       0x02

//***************************************************
// Value Definition for EFI_TIME.TimeZone. See below.
//***************************************************
#define EFI_UNSPECIFIED_TIMEZONE   0x07FF


typedef struct _EFI_TIME EFI_TIME;// This represents the current time information
typedef struct _EFI_TIME_CAPABILITIES EFI_TIME_CAPABILITIES;//This provides the capabilities of the

typedef EFI_STATUS(EFIAPI *EFI_GET_TIME)(EFI_TIME *Time,EFI_TIME_CAPABILITIES *Capabilities);//現在の時刻と日付の情報、およびハードウェア プラットフォームの時刻管理機能を返します。
typedef EFI_STATUS(EFIAPI *EFI_SET_TIME)(EFI_TIME *Time);//現在のローカルの時刻と日付の情報を設定します。
typedef EFI_STATUS(EFIAPI *EFI_GET_WAKEUP_TIME)(BOOLEAN *Enabled,BOOLEAN *Pending,EFI_TIME *Time);//現在の目覚ましアラームクロックの設定を返します。
typedef EFI_STATUS(EFIAPI *EFI_SET_WAKEUP_TIME)(BOOLEAN Enable,EFI_TIME *Time);


typedef struct _EFI_TIME{
	UINT16    Year;              // 1900 - 9999
	UINT8     Month;             // 1 - 12
	UINT8     Day;               // 1 - 31
	UINT8     Hour;              // 0 - 23
	UINT8     Minute;            // 0 - 59
	UINT8     Second;            // 0 - 59
	UINT8     Pad1;
	UINT32    Nanosecond;        // 0 - 999,999,999
	INT16     TimeZone;          // —1440 to 1440 or 2047
	UINT8     Daylight;
	UINT8     Pad2;
} EFI_TIME;

typedef struct _EFI_TIME_CAPABILITIES{
	UINT32                  Resolution;
	UINT32                  Accuracy;
	BOOLEAN                 SetsToZero;
} EFI_TIME_CAPABILITIES;