#pragma once
#include <uefi/uefi_defined.h>

#define EFI_DEVICE_PATH_PROTOCOL_GUID \
	{0x09576e91,0x6d3f,0x11d2,\
		{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}\
	}

#define EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID \
	{0x8b843e20,0x8132,0x4852,\
		{0x90,0xcc,0x55,0x1a,0x4e,0x4a,0x7f,0x1c}\
	}

#define EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID \
	{0x5c99a21,0xc70f,0x4ad2,\
		{0x8a,0x5f,0x35,0xdf,0x33,0x43,0xf5, 0x1e}\
	}
	
#define EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID \
	{0x379be4e,0xd706,0x437d,\
		{0xb0,0x37,0xed,0xb8,0x2f,0xb7,0x72,0xa4 }\
	}

typedef struct _EFI_DEVICE_PATH_PROTOCOL EFI_DEVICE_PATH_PROTOCOL;//任意のデバイス ハンドルで使用して、物理デバイスまたは論理デバイスに関する一般的なパス/場所の情報を取得できます。ハンドルが物理デバイスに論理的にマップされていない場合、ハンドルは必ずしもデバイス パス プロトコルをサポートしない可能性があります。デバイス パスは、ハンドルの対象となるデバイスの場所を示します。デバイス パスのサイズは、デバイス パスを構成する構造から決定できます。
typedef struct _EFI_DEVICE_PATH_TO_TEXT_PROTOCOL EFI_DEVICE_PATH_TO_TEXT_PROTOCOL;//デバイス ノードとパスをテキストに変換します。
typedef struct _EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL;//テキストをデバイス パスとデバイス ノードに変換します。
typedef struct _EFI_DEVICE_PATH_UTILITIES_PROTOCOL EFI_DEVICE_PATH_UTILITIES_PROTOCOL;//デバイス パスとデバイス ノードを作成および操作します。


typedef CHAR16*(EFIAPI *EFI_DEVICE_PATH_TO_TEXT_NODE)(CONST EFI_DEVICE_PATH_PROTOCOL* DeviceNode,BOOLEAN DisplayOnly,BOOLEAN AllowShortcuts);//デバイス ノードをテキスト表現に変換します。
typedef CHAR16*(EFIAPI *EFI_DEVICE_PATH_TO_TEXT_PATH)(CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,BOOLEAN DisplayOnly,BOOLEAN AllowShortcuts);//デバイス パスをテキスト表現に変換します。
typedef EFI_DEVICE_PATH_PROTOCOL*(EFIAPI *EFI_DEVICE_PATH_FROM_TEXT_NODE)(CONST CHAR16* TextDeviceNode);//テキストをデバイス ノードのバイナリ表現に変換します。
typedef EFI_DEVICE_PATH_PROTOCOL*(EFIAPI *EFI_DEVICE_PATH_FROM_TEXT_PATH)(CONST CHAR16* TextDevicePath);//テキストをバイナリ デバイス パス表現に変換します。
typedef UINTN(EFIAPI *EFI_DEVICE_PATH_UTILS_GET_DEVICE_PATH_SIZE)(CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath);//デバイス パスのサイズをバイト単位で返します。
typedef EFI_DEVICE_PATH_PROTOCOL*(EFIAPI *EFI_DEVICE_PATH_UTILS_DUP_DEVICE_PATH)(CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath);//指定されたパスの複製を作成します。
typedef EFI_DEVICE_PATH_PROTOCOL*(EFIAPI *EFI_DEVICE_PATH_UTILS_APPEND_PATH)(CONST EFI_DEVICE_PATH_PROTOCOL *Src1,CONST EFI_DEVICE_PATH_PROTOCOL *Src2);//2 番目のデバイス パスを最初のデバイス パスに追加して新しいパスを作成します。
typedef EFI_DEVICE_PATH_PROTOCOL*(EFIAPI *EFI_DEVICE_PATH_UTILS_APPEND_NODE)(CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,CONST EFI_DEVICE_PATH_PROTOCOL *DeviceNode);//デバイス ノードをデバイス パスに追加して新しいパスを作成します。
typedef EFI_DEVICE_PATH_PROTOCOL*(EFIAPI *EFI_DEVICE_PATH_UTILS_APPEND_INSTANCE)(CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,CONST EFI_DEVICE_PATH_PROTOCOL *DevicePathInstance);//指定されたデバイス パス インスタンスを指定されたデバイス パスに追加して、新しいパスを作成します。
typedef EFI_DEVICE_PATH_PROTOCOL*(EFIAPI *EFI_DEVICE_PATH_UTILS_GET_NEXT_INSTANCE)(EFI_DEVICE_PATH_PROTOCOL **DevicePathInstance,UINTN *DevicePathInstanceSize);//現在のデバイス パス インスタンスのコピーを作成し、次のデバイス パス インスタンスへのポインターを返します。
typedef EFI_DEVICE_PATH_PROTOCOL*(EFIAPI *EFI_DEVICE_PATH_UTILS_CREATE_NODE)(UINT8 NodeType,UINT8 NodeSubType,UINT16 NodeLength);//デバイスノードを作成する
typedef BOOLEAN(EFIAPI *EFI_DEVICE_PATH_UTILS_IS_MULTI_INSTANCE)(CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath);//デバイス パスがマルチインスタンスであるかどうかを返します。
 
 

typedef struct _EFI_DEVICE_PATH_PROTOCOL {
	UINT8           Type;
	UINT8           SubType;
	UINT8           Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

typedef struct _EFI_DEVICE_PATH_TO_TEXT_PROTOCOL {
	EFI_DEVICE_PATH_TO_TEXT_NODE        ConvertDeviceNodeToText;
	EFI_DEVICE_PATH_TO_TEXT_PATH        ConvertDevicePathToText;
}  EFI_DEVICE_PATH_TO_TEXT_PROTOCOL;

typedef struct _EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL {
	EFI_DEVICE_PATH_FROM_TEXT_NODE           ConvertTextToDevicNode;
	EFI_DEVICE_PATH_FROM_TEXT_PATH           ConvertTextToDevicPath;
} EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL;

typedef struct _EFI_DEVICE_PATH_UTILITIES_PROTOCOL {
	EFI_DEVICE_PATH_UTILS_GET_DEVICE_PATH_SIZE    GetDevicePathSize;
	EFI_DEVICE_PATH_UTILS_DUP_DEVICE_PATH         DuplicateDevicePath;
	EFI_DEVICE_PATH_UTILS_APPEND_PATH             AppendDevicePath;
	EFI_DEVICE_PATH_UTILS_APPEND_NODE             AppendDeviceNode;
	EFI_DEVICE_PATH_UTILS_APPEND_INSTANCE         AppendDevicePathInstance;
	EFI_DEVICE_PATH_UTILS_GET_NEXT_INSTANCE       GetNextDevicePathInstance;
	EFI_DEVICE_PATH_UTILS_IS_MULTI_INSTANCE       IsDevicePathMultiInstance;
	EFI_DEVICE_PATH_UTILS_CREATE_NODE             CreateDeviceNode;
}   EFI_DEVICE_PATH_UTILITIES_PROTOCOL;