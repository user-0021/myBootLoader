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

typedef struct _EFI_DEVICE_PATH_PROTOCOL EFI_DEVICE_PATH_PROTOCOL;//任意のデバイス ハンドルで使用して、物理デバイスまたは論理デバイスに関する一般的なパス/場所の情報を取得できます。ハンドルが物理デバイスに論理的にマップされていない場合、ハンドルは必ずしもデバイス パス プロトコルをサポートしない可能性があります。デバイス パスは、ハンドルの対象となるデバイスの場所を示します。デバイス パスのサイズは、デバイス パスを構成する構造から決定できます。
typedef struct _EFI_DEVICE_PATH_TO_TEXT_PROTOCOL EFI_DEVICE_PATH_TO_TEXT_PROTOCOL;//デバイス ノードとパスをテキストに変換します。
typedef struct _EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL;//テキストをデバイス パスとデバイス ノードに変換します。

typedef CHAR16*(EFIAPI *EFI_DEVICE_PATH_TO_TEXT_NODE)(CONST EFI_DEVICE_PATH_PROTOCOL* DeviceNode,BOOLEAN DisplayOnly,BOOLEAN AllowShortcuts);//デバイス ノードをテキスト表現に変換します。
typedef CHAR16*(EFIAPI *EFI_DEVICE_PATH_TO_TEXT_PATH)(CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,BOOLEAN DisplayOnly,BOOLEAN AllowShortcuts);//デバイス パスをテキスト表現に変換します。
typedef EFI_DEVICE_PATH_PROTOCOL*(EFIAPI *EFI_DEVICE_PATH_FROM_TEXT_NODE)(CONST CHAR16* TextDeviceNode);//テキストをデバイス ノードのバイナリ表現に変換します。
typedef EFI_DEVICE_PATH_PROTOCOL*(EFIAPI *EFI_DEVICE_PATH_FROM_TEXT_PATH)(CONST CHAR16* TextDevicePath);//テキストをバイナリ デバイス パス表現に変換します。



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