#pragma once
#include <uefi/uefi_defined.h>
#include <uefi/console/drawing_characters.h>

//*******************************************************
// Attributes
//*******************************************************
#define EFI_BLACK                              0x00
#define EFI_BLUE                               0x01
#define EFI_GREEN                              0x02
#define EFI_CYAN                               0x03
#define EFI_RED                                0x04
#define EFI_MAGENTA                            0x05
#define EFI_BROWN                              0x06
#define EFI_LIGHTGRAY                          0x07
#define EFI_BRIGHT                             0x08
#define EFI_DARKGRAY						   0x08
#define EFI_LIGHTBLUE                          0x09
#define EFI_LIGHTGREEN                         0x0A
#define EFI_LIGHTCYAN                          0x0B
#define EFI_LIGHTRED                           0x0C
#define EFI_LIGHTMAGENTA                       0x0D
#define EFI_YELLOW                             0x0E
#define EFI_WHITE                              0x0F


#define EFI_BACKGROUND_BLACK                   0x00
#define EFI_BACKGROUND_BLUE                    0x10
#define EFI_BACKGROUND_GREEN                   0x20
#define EFI_BACKGROUND_CYAN                    0x30
#define EFI_BACKGROUND_RED                     0x40
#define EFI_BACKGROUND_MAGENTA                 0x50
#define EFI_BACKGROUND_BROWN                   0x60
#define EFI_BACKGROUND_LIGHTGRAY               0x70

//*****************************************************
// EFI_KEY_SHIFT_STATE
//*****************************************************
#define EFI_SHIFT_STATE_VALID          0x80000000
#define EFI_RIGHT_SHIFT_PRESSED        0x00000001
#define EFI_LEFT_SHIFT_PRESSED         0x00000002
#define EFI_RIGHT_CONTROL_PRESSED      0x00000004
#define EFI_LEFT_CONTROL_PRESSED       0x00000008
#define EFI_RIGHT_ALT_PRESSED          0x00000010
#define EFI_LEFT_ALT_PRESSED           0x00000020
#define EFI_RIGHT_LOGO_PRESSED         0x00000040
#define EFI_LEFT_LOGO_PRESSED          0x00000080
#define EFI_MENU_KEY_PRESSED           0x00000100
#define EFI_SYS_REQ_PRESSED            0x00000200

//*****************************************************
// EFI_KEY_TOGGLE_STATE
//*****************************************************
#define EFI_TOGGLE_STATE_VALID 0x80
#define EFI_KEY_STATE_EXPOSED 0x40
#define EFI_SCROLL_LOCK_ACTIVE 0x01
#define EFI_NUM_LOCK_ACTIVE 0x02
#define EFI_CAPS_LOCK_ACTIVE 0x04

//*****************************************************
// EFI_ABSOLUTE_POINTER_MODE_ATTRIBUTES
//*****************************************************
#define EFI_ABSP_SupportsAltActive     0x00000001
#define EFI_ABSP_SupportsPressureAsZ   0x00000002

//*****************************
//definitions of bits within ActiveButtons
//*****************************
#define EFI_ABSP_TouchActive     0x00000001
#define EFI_ABS_AltActive        0x00000002

//******************************************************
// EFI_KEY_TOGGLE_STATE
//******************************************************
typedef UINT8 EFI_KEY_TOGGLE_STATE;



typedef struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;//Simple Text Input Ex プロトコルは、新しい機能を有効にする Simple Text Input プロトコルの拡張機能を定義します。
typedef struct _EFI_KEY_DATA EFI_KEY_DATA;//押されたキーのキーストローク状態データが格納されるバッファ
typedef struct _EFI_KEY_STATE EFI_KEY_STATE;//切り替えられたさまざまな属性と入力修飾子の値
typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;//シンプル テキスト入力プロトコルは、ConsoleIn デバイスをサポートするために必要な最小限の入力を定義します。
typedef struct _EFI_INPUT_KEY EFI_INPUT_KEY;//押されたキーのキーストローク情報が格納されるバッファ
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;//このプロトコルは、テキストベースの出力デバイスを制御するために使用されます。
typedef struct _SIMPLE_TEXT_OUTPUT_MODE SIMPLE_TEXT_OUTPUT_MODE;//インターフェイスのモード。
typedef struct _EFI_SIMPLE_POINTER_PROTOCOL EFI_SIMPLE_POINTER_PROTOCOL;//ポインター デバイスに関する情報を取得できるサービスを提供します。
typedef struct _EFI_SIMPLE_POINTER_MODE EFI_SIMPLE_POINTER_MODE;//EFI_SIMPLE_POINTER_MODEデータへのポインター
typedef struct _EFI_SIMPLE_POINTER_STATE EFI_SIMPLE_POINTER_STATE;//ポインター デバイスの状態情報
typedef struct _EFI_ABSOLUTE_POINTER_PROTOCOL EFI_ABSOLUTE_POINTER_PROTOCOL;//絶対ポインター デバイスに関する情報を取得できるサービスを提供します。
typedef struct _EFI_ABSOLUTE_POINTER_MODE EFI_ABSOLUTE_POINTER_MODE;//絶対ポインターのオプション値
typedef struct _EFI_ABSOLUTE_POINTER_STATE EFI_ABSOLUTE_POINTER_STATE;//ポインター デバイスの状態情報



typedef EFI_STATUS(EFIAPI *EFI_KEY_NOTIFY_FUNCTION)(EFI_KEY_DATA *KeyData);

typedef EFI_STATUS(EFIAPI *EFI_INPUT_RESET_EX)(EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,BOOLEAN ExtendedVerification);//入力デバイスのハードウェアをリセットします。
typedef EFI_STATUS(EFIAPI *EFI_INPUT_READ_KEY_EX)(EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,EFI_KEY_DATA *KeyData);//入力デバイスから次のキーストロークを読み取ります。
typedef EFI_STATUS(EFIAPI *EFI_SET_STATE)(EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,EFI_KEY_TOGGLE_STATE *KeyToggleState);//入力デバイスの特定の状態を設定します。
typedef EFI_STATUS(EFIAPI *EFI_REGISTER_KEYSTROKE_NOTIFY)(EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,EFI_KEY_DATA *KeyData,EFI_KEY_NOTIFY_FUNCTION KeyNotificationFunction,VOID **NotifyHandle);//入力デバイスの特定のキーストロークに対する通知機能を登録します。
typedef EFI_STATUS(EFIAPI *EFI_UNREGISTER_KEYSTROKE_NOTIFY)(EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,VOID *NotificationHandle);//以前に登録された通知を削除します。
typedef EFI_STATUS(EFIAPI *EFI_INPUT_RESET)(EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,BOOLEAN ExtendedVerification);//入力デバイスのハードウェアをリセットします。
typedef EFI_STATUS(EFIAPI *EFI_INPUT_READ_KEY)(EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,EFI_INPUT_KEY *Key);//入力デバイスから次のキーストロークを読み取ります。
typedef EFI_STATUS(EFIAPI *EFI_TEXT_RESET)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,BOOLEAN ExtendedVerification);//テキスト出力デバイスのハードウェアをリセットします。
typedef EFI_STATUS(EFIAPI *EFI_TEXT_STRING)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,CHAR16 *String);//文字列を出力デバイスに書き込みます。
typedef EFI_STATUS(EFIAPI *EFI_TEXT_TEST_STRING)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,CHAR16 *String);//文字列内のすべての文字がターゲット デバイスに出力できることを確認します。
typedef EFI_STATUS(EFIAPI *EFI_TEXT_QUERY_MODE)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,UINTN ModeNumber,UINTN *Columns,UINTN *Rows);//出力デバイスがサポートする使用可能なテキスト モードの情報を返します。
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_MODE)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,UINTN ModeNumber);//出力デバイスを指定されたモードに設定します。
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,UINTN Attribute);//背景色と前景色を設定します。
typedef EFI_STATUS(EFIAPI *EFI_TEXT_CLEAR_SCREEN)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);//出力デバイスの表示を現在選択されている背景色にクリアします。
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_CURSOR_POSITION)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,UINTN Column,UINTN Row);//カーソル位置の現在の座標を設定します。
typedef EFI_STATUS(EFIAPI *EFI_TEXT_ENABLE_CURSOR)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,BOOLEAN Visible);//カーソルを表示または非表示にします。
typedef EFI_STATUS(EFIAPI *EFI_SIMPLE_POINTER_RESET)(EFI_SIMPLE_POINTER_PROTOCOL *This,BOOLEAN ExtendedVerification);//ポインター デバイスのハードウェアをリセットします。
typedef EFI_STATUS(EFIAPI *EFI_SIMPLE_POINTER_GET_STATE)(EFI_SIMPLE_POINTER_PROTOCOL *This,EFI_SIMPLE_POINTER_STATE *State);//ポインター デバイスの現在の状態を取得します。
typedef EFI_STATUS(EFIAPI *EFI_ABSOLUTE_POINTER_RESET)(EFI_ABSOLUTE_POINTER_PROTOCOL *This,BOOLEAN ExtendedVerification);//ポインター デバイスのハードウェアをリセットします。
typedef EFI_STATUS(EFIAPI *EFI_ABSOLUTE_POINTER_GET_STATE)(EFI_ABSOLUTE_POINTER_PROTOCOL *This,EFI_ABSOLUTE_POINTER_STATE *State);//ポインター デバイスの現在の状態を取得します。



typedef struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL{
	EFI_INPUT_RESET_EX                          Reset;
	EFI_INPUT_READ_KEY_EX                       ReadKeyStrokeEx;
	EFI_EVENT                                   WaitForKeyEx;
	EFI_SET_STATE                               SetState;
	EFI_REGISTER_KEYSTROKE_NOTIFY               RegisterKeyNotify;
	EFI_UNREGISTER_KEYSTROKE_NOTIFY             UnregisterKeyNotify;
} EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;

typedef struct _EFI_INPUT_KEY{
	UINT16                             ScanCode;
	CHAR16                             UnicodeChar;
} EFI_INPUT_KEY;

typedef struct _EFI_KEY_STATE{
	UINT32                       KeyShiftState;
	EFI_KEY_TOGGLE_STATE         KeyToggleState;
} EFI_KEY_STATE;

typedef struct _EFI_KEY_DATA{
	EFI_INPUT_KEY           Key;
	EFI_KEY_STATE           KeyState;
} EFI_KEY_DATA;

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL{
	EFI_INPUT_RESET                       Reset;
	EFI_INPUT_READ_KEY                    ReadKeyStroke;
	EFI_EVENT                             WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL{
	EFI_TEXT_RESET                           Reset;
	EFI_TEXT_STRING                          OutputString;
	EFI_TEXT_TEST_STRING                     TestString;
	EFI_TEXT_QUERY_MODE                      QueryMode;
	EFI_TEXT_SET_MODE                        SetMode;
	EFI_TEXT_SET_ATTRIBUTE                   SetAttribute;
	EFI_TEXT_CLEAR_SCREEN                    ClearScreen;
	EFI_TEXT_SET_CURSOR_POSITION             SetCursorPosition;
	EFI_TEXT_ENABLE_CURSOR                   EnableCursor;
	SIMPLE_TEXT_OUTPUT_MODE                  *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct _SIMPLE_TEXT_OUTPUT_MODE{
	INT32                              MaxMode;
	// current settings
	INT32                              Mode;
	INT32                              Attribute;
	INT32                              CursorColumn;
	INT32                              CursorRow;
	BOOLEAN                            CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef struct _EFI_SIMPLE_POINTER_PROTOCOL{
	EFI_SIMPLE_POINTER_RESET                    Reset;
	EFI_SIMPLE_POINTER_GET_STATE                GetState;
	EFI_EVENT                                   WaitForInput;
	EFI_SIMPLE_POINTER_MODE                       *Mode;
} EFI_SIMPLE_POINTER_PROTOCOL;

typedef struct _EFI_SIMPLE_POINTER_MODE{
	UINT64                    ResolutionX;
	UINT64                    ResolutionY;
	UINT64                    ResolutionZ;
	BOOLEAN                   LeftButton;
	BOOLEAN                   RightButton;
} EFI_SIMPLE_POINTER_MODE;

typedef struct _EFI_SIMPLE_POINTER_STATE{
	INT32                  RelativeMovementX;
	INT32                  RelativeMovementY;
	INT32                  RelativeMovementZ;
	BOOLEAN                LeftButton;
	BOOLEAN                RightButton;
}EFI_SIMPLE_POINTER_STATE;

typedef struct _EFI_ABSOLUTE_POINTER_PROTOCOL {
	EFI_ABSOLUTE_POINTER_RESET                     Reset;
	EFI_ABSOLUTE_POINTER_GET_STATE                 GetState;
	EFI_EVENT                                      WaitForInput;
	EFI_ABSOLUTE_POINTER_MODE                      *Mode;
} EFI_ABSOLUTE_POINTER_PROTOCOL;

typedef struct _EFI_ABSOLUTE_POINTER_MODE{
	UINT64                     AbsoluteMinX;
	UINT64                     AbsoluteMinY;
	UINT64                     AbsoluteMinZ;
	UINT64                     AbsoluteMaxX;
	UINT64                     AbsoluteMaxY;
	UINT64                     AbsoluteMaxZ;
	UINT32                     Attributes;
} EFI_ABSOLUTE_POINTER_MODE;

typedef struct _EFI_ABSOLUTE_POINTER_STATE{
    UINT64                 CurrentX;
    UINT64                 CurrentY;
    UINT64                 CurrentZ;
    UINT32                 ActiveButtons;
} EFI_ABSOLUTE_POINTER_STATE;
