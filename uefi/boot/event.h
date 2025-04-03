#pragma once
#include <uefi/uefi_defined.h>

//***************************************************
// EFI_EVENT
//******************************************************
typedef VOID *EFI_EVENT;

//******************************************************
// Event Types
//******************************************************
// These types can be "ORed" together as needed - for example,
// EVT_TIMER might be "Ored" with EVT_NOTIFY_WAIT or
// EVT_NOTIFY_SIGNAL.
#define EVT_TIMER                            0x80000000
#define EVT_RUNTIME                          0x40000000

#define EVT_NOTIFY_WAIT                      0x00000100
#define EVT_NOTIFY_SIGNAL                    0x00000200

#define EVT_SIGNAL_EXIT_BOOT_SERVICES        0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE    0x60000202

//******************************************************
// EFI_TPL
//******************************************************
typedef UINTN EFI_TPL;

//******************************************************
// Task Priority Levels
//******************************************************
#define TPL_APPLICATION    4
#define TPL_CALLBACK       8
#define TPL_NOTIFY         16
#define TPL_HIGH_LEVEL     31

//******************************************************
//EFI_TIMER_DELAY
//******************************************************
typedef enum {
	TimerCancel,
	TimerPeriodic,
	TimerRelative
} EFI_TIMER_DELAY;


typedef VOID(EFIAPI *EFI_EVENT_NOTIFY)(EFI_EVENT Event,VOID *Context);//イベントの通知関数
typedef EFI_STATUS(EFIAPI *EFI_CREATE_EVENT)(UINT32 Type,EFI_TPL NotifyTpl,EFI_EVENT_NOTIFY NotifyFunction,VOID *NotifyContext,EFI_EVENT *Event);//イベントを作成します。
typedef EFI_STATUS(EFIAPI *EFI_CREATE_EVENT_EX)(UINT32 Type,EFI_TPL NotifyTpl,EFI_EVENT_NOTIFY NotifyFunction,CONST VOID *NotifyContext,CONST EFI_GUID *EventGroup,EFI_EVENT *Event);//グループ内にイベントを作成します。
typedef EFI_STATUS(EFIAPI *EFI_CLOSE_EVENT)(EFI_EVENT Event);//イベントを終了します。
typedef EFI_STATUS(EFIAPI *EFI_SIGNAL_EVENT)(EFI_EVENT Event);//イベントを通知します。
typedef EFI_STATUS(EFIAPI *EFI_WAIT_FOR_EVENT)(UINTN NumberOfEvents,EFI_EVENT *Event,UINTN *Index);//イベントが通知されるまで実行を停止します。
typedef EFI_STATUS(EFIAPI *EFI_CHECK_EVENT)(EFI_EVENT Event);//イベントがシグナル状態にあるかどうかを確認します。
typedef EFI_STATUS(EFIAPI *EFI_SET_TIMER)(EFI_EVENT Event,EFI_TIMER_DELAY Type,UINT64 TriggerTime);//タイマーのタイプとタイマー イベントのトリガー時間を設定します。
typedef EFI_TPL(EFIAPI *EFI_RAISE_TPL)(EFI_TPL NewTpl);//タスクの優先度レベルを上げ、以前のレベルに戻します。
typedef VOID(EFIAPI *EFI_RESTORE_TPL)(EFI_TPL OldTpl);//タスクの優先度レベルを以前の値に戻します。

