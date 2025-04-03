#pragma once
#include <uefi/uefi_defined.h>


//******************************************************
// CONTROL BITS
//******************************************************

#define EFI_SERIAL_CLEAR_TO_SEND                0x0010
#define EFI_SERIAL_DATA_SET_READY               0x0020
#define EFI_SERIAL_RING_INDICATE                0x0040
#define EFI_SERIAL_CARRIER_DETECT               0x0080
#define EFI_SERIAL_REQUEST_TO_SEND              0x0002
#define EFI_SERIAL_DATA_TERMINAL_READY          0x0001
#define EFI_SERIAL_INPUT_BUFFER_EMPTY           0x0100
#define EFI_SERIAL_OUTPUT_BUFFER_EMPTY          0x0200
#define EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE     0x1000
#define EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE     0x2000
#define EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE 0x4000


//******************************************************
// EFI_PARITY_TYPE
//******************************************************
typedef enum {
	DefaultParity,
	NoParity,
	EvenParity,
	OddParity,
	MarkParity,
	SpaceParity
} EFI_PARITY_TYPE;
   
//******************************************************
// EFI_STOP_BITS_TYPE
//******************************************************
typedef enum {
	DefaultStopBits,
	OneStopBit,         // 1 stop bit
	OneFiveStopBits,    // 1.5 stop bits
	TwoStopBits         // 2 stop bits
} EFI_STOP_BITS_TYPE;


typedef struct _EFI_SERIAL_IO_PROTOCOL EFI_SERIAL_IO_PROTOCOL;//このプロトコルは、あらゆるタイプの文字ベースの I/O デバイスとの通信に使用されます。
typedef struct _SERIAL_IO_MODE SERIAL_IO_MODE;//プロトコルのオプション値

typedef EFI_STATUS(EFIAPI *EFI_SERIAL_RESET)(EFI_SERIAL_IO_PROTOCOL *This);//シリアルデバイスをリセットします。
typedef EFI_STATUS(EFIAPI *EFI_SERIAL_SET_ATTRIBUTES)(EFI_SERIAL_IO_PROTOCOL *This,UINT64 BaudRate,UINT32 ReceiveFifoDepth,UINT32 Timeout,EFI_PARITY_TYPE Parity,UINT8 DataBits,EFI_STOP_BITS_TYPE StopBits);//シリアル デバイスのボー レート、受信 FIFO 深度、送信/受信タイムアウト、パリティ、データ ビット、ストップ ビットを設定します。
typedef EFI_STATUS(EFIAPI *EFI_SERIAL_SET_CONTROL_BITS)(EFI_SERIAL_IO_PROTOCOL *This,UINT32 Control);//シリアルデバイスの制御ビットを設定します。
typedef EFI_STATUS(EFIAPI *EFI_SERIAL_GET_CONTROL_BITS)(EFI_SERIAL_IO_PROTOCOL *This,UINT32 *Control);//シリアル デバイス上の制御ビットのステータスを取得します。
typedef EFI_STATUS(EFIAPI *EFI_SERIAL_WRITE)(EFI_SERIAL_IO_PROTOCOL *This,UINTN *BufferSize,VOID *Buffer);//シリアルデバイスにデータを書き込みます。
typedef EFI_STATUS(EFIAPI *EFI_SERIAL_READ)(EFI_SERIAL_IO_PROTOCOL *This,UINTN *BufferSize,VOID *Buffer);//シリアルデバイスからデータを読み取ります。

typedef struct _EFI_SERIAL_IO_PROTOCOL{
	UINT32                       Revision;
	EFI_SERIAL_RESET             Reset;
	EFI_SERIAL_SET_ATTRIBUTES    SetAttributes;
	EFI_SERIAL_SET_CONTROL_BITS  SetControl;
	EFI_SERIAL_GET_CONTROL_BITS  GetControl;
	EFI_SERIAL_WRITE             Write;
	EFI_SERIAL_READ              Read;
	SERIAL_IO_MODE               *Mode;
	CONST EFI_GUID               *DeviceTypeGuid; // Revision 1.1
} EFI_SERIAL_IO_PROTOCOL;

typedef struct _SERIAL_IO_MODE{
	UINT32 ControlMask;

	// current Attributes
	UINT32            Timeout;
	UINT64            BaudRate;
	UINT32            ReceiveFifoDepth;
	UINT32            DataBits;
	UINT32            Parity;
	UINT32            StopBits;
} SERIAL_IO_MODE;
