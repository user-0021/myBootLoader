#pragma once
#include <uefi/uefi_defined.h>

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID {\
	0x9042a9de,0x23dc,0x4a38,\
		{0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}\
	}

#define EFI_EDID_OVERRIDE_DONT_OVERRIDE 0x01
#define EFI_EDID_OVERRIDE_ENABLE_HOT_PLUG 0x02

typedef enum {
	PixelRedGreenBlueReserved8BitPerColor,
	PixelBlueGreenRedReserved8BitPerColor,
	PixelBitMask,
	PixelBltOnly,
	PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;
   
typedef enum {
	EfiBltVideoFill,
	EfiBltVideoToBltBuffer,
	EfiBltBufferToVideo,
	EfiBltVideoToVideo,
	EfiGraphicsOutputBltOperationMax
} EFI_GRAPHICS_OUTPUT_BLT_OPERATION;


typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL EFI_GRAPHICS_OUTPUT_PROTOCOL;//ビデオ モードを設定し、グラフィックス コントローラーのフレーム バッファーとの間でピクセルをコピーするための基本的な抽象化を提供しま
typedef struct _EFI_PIXEL_BITMASK EFI_PIXEL_BITMASK;//ビットのカラーのマスク値
typedef struct _EFI_GRAPHICS_OUTPUT_MODE_INFORMATION EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;//バッファのオプション値
typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;//ピクセルをフレーム バッファーに直接描画できるようにするソフトウェア抽象化を提供します
typedef struct _EFI_GRAPHICS_OUTPUT_BLT_PIXEL EFI_GRAPHICS_OUTPUT_BLT_PIXEL;//ピクセルのカラー情報です
typedef struct _EFI_EDID_DISCOVERED_PROTOCOL EFI_EDID_DISCOVERED_PROTOCOL;//このプロトコルには、ビデオ出力デバイスから取得された EDID 情報が含まれています。
typedef struct _EFI_EDID_ACTIVE_PROTOCOL EFI_EDID_ACTIVE_PROTOCOL;//このプロトコルには、アクティブなビデオ出力デバイスの EDID 情報が含まれています。これは、オーバーライドが利用可能な場合は EFI_EDID_OVERRIDE_PROTOCOL から取得された EDID 情報、オーバーライドが利用できない場合は EFI_EDID_DISCOVERED_PROTOCOL からの EDID 情報の同一コピーのいずれかです。
typedef struct _EFI_EDID_OVERRIDE_PROTOCOL EFI_EDID_OVERRIDE_PROTOCOL;//このプロトコルは、プラットフォームがグラフィックス出力プロトコルのプロデューサーに EDID 情報を提供できるようにするためにプラットフォームによって生成されます。


typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This,UINT32 ModeNumber,UINTN *SizeOfInfo,EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info);//グラフィックス デバイスとアクティブなビデオ出力デバイスのセットがサポートする使用可能なグラフィックス モードの情報を返します。
typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This,UINT32 ModeNumber);//ビデオ デバイスを指定されたモードに設定し、出力ディスプレイの表示部分を黒くクリアします。
typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This,EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer,EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,UINTN SourceX,UINTN SourceY,UINTN DestinationX,UINTN DestinationY,UINTN Width,UINTN Height,UINTN Delta);//グラフィック画面上のピクセルの四角形を Blt します。Blt は BLock Transfer の略です。
typedef EFI_STATUS(EFIAPI *EFI_EDID_OVERRIDE_PROTOCOL_GET_EDID)(EFI_EDID_OVERRIDE_PROTOCOL *This,EFI_HANDLE *ChildHandle,UINT32 *Attributes,UINTN *EdidSize,UINT8 **Edid);//指定されたビデオ出力デバイスのポリシー情報と、場合によっては置換 EDID を返します。

typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL {
	EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE     QueryMode;
	EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE       SetMode;
	EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT            Blt;
	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE           *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef struct _EFI_PIXEL_BITMASK{
	UINT32              RedMask;
	UINT32              GreenMask;
	UINT32              BlueMask;
	UINT32              ReservedMask;
} EFI_PIXEL_BITMASK;

typedef struct _EFI_GRAPHICS_OUTPUT_MODE_INFORMATION{
	UINT32                    Version;
	UINT32                    HorizontalResolution;
	UINT32                    VerticalResolution;
	EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
	EFI_PIXEL_BITMASK         PixelInformation;
	UINT32                    PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE{
	UINT32                                    MaxMode;
	UINT32                                    Mode;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION      *Info;
   UINTN                                      SizeOfInfo;
	EFI_PHYSICAL_ADDRESS                      FrameBufferBase;
	UINTN                                     FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct _EFI_GRAPHICS_OUTPUT_BLT_PIXEL{
	UINT8                        Blue;
	UINT8                        Green;
	UINT8                        Red;
	UINT8                        Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

typedef struct _EFI_EDID_DISCOVERED_PROTOCOL{
	UINT32                              SizeOfEdid;
	UINT8                               *Edid;
} EFI_EDID_DISCOVERED_PROTOCOL;

typedef struct _EFI_EDID_ACTIVE_PROTOCOL{
	UINT32                             SizeOfEdid;
	UINT8                              *Edid;
} EFI_EDID_ACTIVE_PROTOCOL;

typedef struct _EFI_EDID_OVERRIDE_PROTOCOL {
	EFI_EDID_OVERRIDE_PROTOCOL_GET_EDID      GetEdid;
} EFI_EDID_OVERRIDE_PROTOCOL;
   