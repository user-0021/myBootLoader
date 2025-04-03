#pragma once
#include <stdint.h>
#include <stddef.h>

#define HIGH_BIT_SET(value) (value | 0x8000000000000000)
#define HIGH_BIT_CLEAR(value) (value & ~0x8000000000000000)

#define EFIAPI
#define CONST const

#define TRUE 1
#define FALSE 0

typedef uint64_t UINTN;	 //ネイティブ幅の符号なし値。
typedef int8_t INT8;	 //1 バイトの符号付き値。
typedef uint8_t UINT8; 	 //1 バイトの符号なし値。
typedef int16_t INT16;	 //2 バイトの符号付き値。
typedef uint16_t UINT16; //2 バイトの符号なし値。
typedef int32_t  INT32;	 //4 バイトの符号付き値。
typedef uint32_t UINT32; //4 バイトの符号なし値。
typedef int64_t  INT64;	 //8 バイトの符号付き値。
typedef uint64_t UINT64; //8 バイトの符号なし値。
typedef wchar_t CHAR16; //2 バイト文字。特に指定がない限り、すべての文字と文字列は、Unicode 2.1 および ISO/IEC 10646 標準で定義されている UCS-2 エンコード形式で保存されます。
typedef void VOID;		 //宣言されていない型です。
typedef UINTN EFI_STATUS;//ステータス コード。UINTN と入力します。
typedef VOID* EFI_HANDLE;//関連するインターフェースのコレクション。VOID * です。
typedef VOID* EFI_EVENT; //イベント構造体へのハンドル。VOID * です。
typedef char BOOLEAN;	 //論理ブール値。FALSEの場合は 0 、 TRUEの場合は 1 を含む 1 バイト値。その他の値は未定義です。
typedef UINT64 EFI_PHYSICAL_ADDRESS;//物理アドレス
typedef UINT64 EFI_VIRTUAL_ADDRESS;//仮想アドレス

/****************Success Codes*****************/
#define EFI_SUCCESS 			 	HIGH_BIT_CLEAR(0)//操作は正常に完了しました。

/******************Error Codes*****************/
#define EFI_LOAD_ERROR 			 	HIGH_BIT_SET(1)	//画像の読み込みに失敗しました。
#define EFI_INVALID_PARAMETER	 	HIGH_BIT_SET(2)	//パラメータが正しくありませんでした。
#define EFI_UNSUPPORTED			 	HIGH_BIT_SET(3)	//この操作はサポートされていません。
#define EFI_BAD_BUFFER_SIZE		 	HIGH_BIT_SET(4)	//バッファが要求に対して適切なサイズではありませんでした。
#define EFI_BUFFER_TOO_SMALL	 	HIGH_BIT_SET(5)	//バッファは要求されたデータを保持するのに十分な大きさではありません。このエラーが発生すると、適切なパラメータで必要なバッファ サイズが返されます。
#define EFI_NOT_READY			 	HIGH_BIT_SET(6)	//返却時に保留中のデータはありません。
#define EFI_DEVICE_ERROR		 	HIGH_BIT_SET(7)	//操作の試行中に物理デバイスがエラーを報告しました。
#define EFI_WRITE_PROTECTED		 	HIGH_BIT_SET(8)	//デバイスに書き込むことができません。
#define EFI_OUT_OF_RESOURCES	 	HIGH_BIT_SET(9)	//リソースが不足しています。
#define EFI_VOLUME_CORRUPTED	 	HIGH_BIT_SET(10)	//ファイル システムに不整合が検出されたため、操作が失敗しました。
#define EFI_VOLUME_FULL			 	HIGH_BIT_SET(11)	//ファイル システムに空き領域がありません。
#define EFI_NO_MEDIA			 	HIGH_BIT_SET(12)	//デバイスには操作を実行するための媒体が含まれていません。
#define EFI_MEDIA_CHANGED		 	HIGH_BIT_SET(13)	//前回のアクセス以降、デバイス内のメディアが変更されました。
#define EFI_NOT_FOUND			 	HIGH_BIT_SET(14)	//アイテムが見つかりませんでした。
#define EFI_ACCESS_DENIED		 	HIGH_BIT_SET(15)	//アクセスが拒否されました。
#define EFI_NO_RESPONSE			 	HIGH_BIT_SET(16)	//サーバーが見つからなかったか、要求に応答しませんでした。
#define EFI_NO_MAPPING			 	HIGH_BIT_SET(17)	//デバイスへのマッピングが存在しません。
#define EFI_TIMEOUT				 	HIGH_BIT_SET(18)	//タイムアウト時間が経過しました。
#define EFI_NOT_STARTED			 	HIGH_BIT_SET(19)	//プロトコルはまだ開始されていません。
#define EFI_ALREADY_STARTED		 	HIGH_BIT_SET(20)	//プロトコルはすでに開始されています。
#define EFI_ABORTED				 	HIGH_BIT_SET(21)	//操作は中止されました。
#define EFI_ICMP_ERROR			 	HIGH_BIT_SET(22)	//ネットワーク操作中に ICMP エラーが発生しました。
#define EFI_TFTP_ERROR			 	HIGH_BIT_SET(23)	//ネットワーク操作中に TFTP エラーが発生しました。
#define EFI_PROTOCOL_ERROR		 	HIGH_BIT_SET(24)	//ネットワーク操作中にプロトコル エラーが発生しました。
#define EFI_INCOMPATIBLE_VERSION 	HIGH_BIT_SET(25)	//関数は、呼び出し元によって要求されたバージョンと互換性のない内部バージョンを検出しました。
#define EFI_SECURITY_VIOLATION	 	HIGH_BIT_SET(26)	//セキュリティ違反のため、機能は実行されませんでした。
#define EFI_CRC_ERROR			 	HIGH_BIT_SET(27)	//CRC エラーが検出されました。
#define EFI_END_OF_MEDIA		 	HIGH_BIT_SET(28)	//メディアの先頭または末尾に到達しました
#define EFI_END_OF_FILE			 	HIGH_BIT_SET(31)	//ファイルの終わりに達しました。
#define EFI_INVALID_LANGUAGE	 	HIGH_BIT_SET(32)	//指定された言語は無効です。
#define EFI_COMPROMISED_DATA		HIGH_BIT_SET(33)	//データのセキュリティ ステータスが不明または侵害されているため、有効なセキュリティ ステータスを復元するにはデータを更新または置き換える必要があります。
#define EFI_IP_ADDRESS_CONFLICT		HIGH_BIT_SET(34)	//アドレス競合アドレス割り当てがあります
#define EFI_HTTP_ERROR				HIGH_BIT_SET(35) //UINT16ハンドルは閉じられましたが、ファイルへのデータは適切にフラッシュされませんでした。
#define EFI_WARN_BUFFER_TOO_SMALL	HIGH_BIT_CLEAR(4)//結果のバッファが小さすぎたため、データはバッファ サイズに切り捨てられました。
#define EFI_WARN_STALE_DATA			HIGH_BIT_CLEAR(5)//このタイプのデータについては、ローカル ポリシーで設定された期間内にデータが更新されていません。
#define EFI_WARN_FILE_SYSTEM		HIGH_BIT_CLEAR(6)//結果のバッファには UEFI 準拠のファイル システムが含まれます。
#define EFI_WARN_RESET_REQUIRED		HIGH_BIT_CLEAR(7)//操作はシステムリセット後に処理されます。


typedef struct _EFI_TABLE_HEADER EFI_TABLE_HEADER; //すべての標準 EFI テーブル タイプに先行するデータ構造。
typedef struct _EFI_GUID EFI_GUID; //一意の識別子値を含む 128 ビット バッファー。特に指定がない限り、64 ビット境界に揃えられます。
typedef struct _EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE; //efi system table


typedef struct _EFI_TABLE_HEADER{
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} EFI_TABLE_HEADER;


typedef struct _EFI_GUID{
	UINT32      Data1;
	UINT16      Data2;
	UINT16      Data3;
	UINT8       Data4[8];
} EFI_GUID;