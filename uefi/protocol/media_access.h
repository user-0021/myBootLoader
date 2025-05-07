#pragma once
#include <uefi/uefi_defined.h>
#include <uefi/protocol/device.h>

#define EFI_LOAD_FILE_PROTOCOL_GUID \
	{0x56EC3091,0x954C,0x11d2,\
		{0x8e,0x3f,0x00,0xa0, 0xc9,0x69,0x72,0x3b}\
	}	

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID \
	{0x0964e5b22,0x6459,0x11d2,\
		{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}\
	}

#define EFI_FILE_INFO_ID \
	{0x09576e92,0x6d3f,0x11d2,\
		{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}\
	}


//FILE PRTOTOCOL REVISION
#define EFI_FILE_PROTOCOL_REVISION           0x00010000
#define EFI_FILE_PROTOCOL_REVISION2          0x00020000
#define EFI_FILE_PROTOCOL_LATEST_REVISION EFI_FILE_PROTOCOL_REVISION2

//******************************************************
// Open Modes
//******************************************************
#define EFI_FILE_MODE_READ       0x0000000000000001
#define EFI_FILE_MODE_WRITE      0x0000000000000002
#define EFI_FILE_MODE_CREATE     0x8000000000000000

//******************************************************
// File Attributes
//******************************************************
#define EFI_FILE_READ_ONLY       0x0000000000000001
#define EFI_FILE_HIDDEN          0x0000000000000002
#define EFI_FILE_SYSTEM          0x0000000000000004
#define EFI_FILE_RESERVED        0x0000000000000008
#define EFI_FILE_DIRECTORY       0x0000000000000010
#define EFI_FILE_ARCHIVE         0x0000000000000020
#define EFI_FILE_VALID_ATTR      0x0000000000000037

typedef struct _EFI_LOAD_FILE_PROTOCOL EFI_LOAD_FILE_PROTOCOL; //任意のデバイスから、主にブート オプションであるファイルを取得するために使用されます。
typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;//デバイスへのファイル タイプ アクセス用の最小限のインターフェイスを提供します。
typedef struct _EFI_FILE_PROTOCOL EFI_FILE_PROTOCOL;//サポートされているファイル システムへのファイル ベースのアクセスを提供します。
typedef struct _EFI_FILE_IO_TOKEN EFI_FILE_IO_TOKEN;//トランザクションのトークン
typedef struct _EFI_FILE_INFO EFI_FILE_INFO;//一般的なファイル情報を設定または取得するためにEFI_FILE_PROTOCOL.SetInfo()およびEFI_FILE_PROTOCOL.GetInfo()で使用できる GUID とデータ構造を提供します。


typedef EFI_STATUS(EFIAPI *EFI_LOAD_FILE)(EFI_LOAD_FILE_PROTOCOL *This,EFI_DEVICE_PATH_PROTOCOL *FilePath,BOOLEAN BootPolicy,UINTN *BufferSize,VOID *Buffer);//ドライバーが指定されたファイルをロードするようにします。
typedef EFI_STATUS(EFIAPI *EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME)(EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,EFI_FILE_PROTOCOL **Root);//ボリューム上のルート ディレクトリを開きます。
typedef EFI_STATUS(EFIAPI *EFI_FILE_OPEN)(EFI_FILE_PROTOCOL *This,EFI_FILE_PROTOCOL **NewHandle,CHAR16 *FileName,UINT64 OpenMode,UINT64 Attributes);//ソース ファイルの場所を基準にして新しいファイルを開きます。
typedef EFI_STATUS(EFIAPI *EFI_FILE_CLOSE)(EFI_FILE_PROTOCOL *This);//指定されたファイル ハンドルを閉じます。
typedef EFI_STATUS(EFIAPI *EFI_FILE_DELETE)(EFI_FILE_PROTOCOL *This);//ファイルを閉じて削除します。
typedef EFI_STATUS(EFIAPI *EFI_FILE_READ)(EFI_FILE_PROTOCOL *This,UINTN *BufferSize,VOID *Buffer);//ファイルからデータを読み取ります。
typedef EFI_STATUS(EFIAPI *EFI_FILE_WRITE)(EFI_FILE_PROTOCOL *This,UINTN *BufferSize,VOID *Buffer);//データをファイルに書き込みます。
typedef EFI_STATUS(EFIAPI *EFI_FILE_OPEN_EX)(EFI_FILE_PROTOCOL *This,EFI_FILE_PROTOCOL **NewHandle,CHAR16 *FileName,UINT64 OpenMode,UINT64 Attributes,EFI_FILE_IO_TOKEN *Token);//ソース ディレクトリの場所を基準にして新しいファイルを開きます。
typedef EFI_STATUS(EFIAPI *EFI_FILE_READ_EX)(EFI_FILE_PROTOCOL *This,EFI_FILE_IO_TOKEN *Token);//ファイルからデータを読み取ります。
typedef EFI_STATUS(EFIAPI *EFI_FILE_WRITE_EX)(EFI_FILE_PROTOCOL *This,EFI_FILE_IO_TOKEN *Token);//データをファイルに書き込みます。
typedef EFI_STATUS(EFIAPI *EFI_FILE_FLUSH_EX)(EFI_FILE_PROTOCOL *This,EFI_FILE_IO_TOKEN *Token);//ファイルに関連付けられたすべての変更されたデータをデバイスにフラッシュします。
typedef EFI_STATUS(EFIAPI *EFI_FILE_SET_POSITION)(EFI_FILE_PROTOCOL *This,UINT64 Position);//ファイルの現在の位置を設定します。
typedef EFI_STATUS(EFIAPI *EFI_FILE_GET_POSITION)(EFI_FILE_PROTOCOL *This,UINT64 *Position);//ファイルの現在の位置を返します。
typedef EFI_STATUS(EFIAPI *EFI_FILE_GET_INFO)(EFI_FILE_PROTOCOL *This,EFI_GUID *InformationType,UINTN *BufferSize,VOID *Buffer);//ファイルに関する情報を返します。
typedef EFI_STATUS(EFIAPI *EFI_FILE_SET_INFO)(EFI_FILE_PROTOCOL *This,EFI_GUID *InformationType,UINTN BufferSize,VOID *Buffer);//ファイルに関する情報を設定します。
typedef EFI_STATUS(EFIAPI *EFI_FILE_FLUSH)(EFI_FILE_PROTOCOL *This);//ファイルに関連付けられたすべての変更されたデータをデバイスにフラッシュします。


typedef struct _EFI_LOAD_FILE_PROTOCOL {
	EFI_LOAD_FILE                       LoadFile;
} EFI_LOAD_FILE_PROTOCOL;

typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
	UINT64                                         Revision;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME    OpenVolume;
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef struct _EFI_FILE_PROTOCOL {
	UINT64                          Revision;
	EFI_FILE_OPEN                   Open;
	EFI_FILE_CLOSE                  Close;
	EFI_FILE_DELETE                 Delete;
	EFI_FILE_READ                   Read;
	EFI_FILE_WRITE                  Write;
	EFI_FILE_GET_POSITION           GetPosition;
	EFI_FILE_SET_POSITION           SetPosition;
	EFI_FILE_GET_INFO               GetInfo;
	EFI_FILE_SET_INFO               SetInfo;
	EFI_FILE_FLUSH                  Flush;
	EFI_FILE_OPEN_EX                OpenEx; // Added for revision 2
	EFI_FILE_READ_EX                ReadEx; // Added for revision 2
	EFI_FILE_WRITE_EX               WriteEx; // Added for revision 2
	EFI_FILE_FLUSH_EX               FlushEx; // Added for revision 2
} EFI_FILE_PROTOCOL;

typedef struct _EFI_FILE_IO_TOKEN{
	EFI_EVENT                         Event;
	EFI_STATUS                        Status;
	UINTN                             BufferSize;
	VOID                              *Buffer;
} EFI_FILE_IO_TOKEN;

typedef struct _EFI_FILE_INFO{
	UINT64                         Size;
	UINT64                         FileSize;
	UINT64                         PhysicalSize;
	EFI_TIME                       CreateTime;
	EFI_TIME                       LastAccessTime;
	EFI_TIME                       ModificationTime;
	UINT64                         Attribute;
	CHAR16                         FileName [];
} EFI_FILE_INFO;
  