#pragma once
#include <uefi/uefi_defined.h>
#include <uefi/runtime/time.h>

//******************************************************
// EFI_RESET_TYPE
//******************************************************
typedef enum {
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown,
	EfiResetPlatformSpecific
}   EFI_RESET_TYPE;


typedef struct _EFI_CAPSULE_BLOCK_DESCRIPTOR EFI_CAPSULE_BLOCK_DESCRIPTOR;//
typedef struct _EFI_CAPSULE_HEADER EFI_CAPSULE_HEADER;//
typedef struct _EFI_CAPSULE_TABLE EFI_CAPSULE_TABLE;//
typedef struct _EFI_MEMORY_RANGE EFI_MEMORY_RANGE;//
typedef struct _EFI_MEMORY_RANGE_CAPSULE EFI_MEMORY_RANGE_CAPSULE;//
typedef struct _EFI_MEMORY_RANGE_CAPSULE_RESULT EFI_MEMORY_RANGE_CAPSULE_RESULT;//
typedef struct _EFI_CAPSULE_RESULT_VARIABLE_HEADER EFI_CAPSULE_RESULT_VARIABLE_HEADER;//
typedef struct _EFI_CAPSULE_RESULT_VARIABLE_FMP EFI_CAPSULE_RESULT_VARIABLE_FMP;//
typedef struct _EFI_CAPSULE_RESULT_VARIABLE_JSON EFI_CAPSULE_RESULT_VARIABLE_JSON;//

typedef VOID(EFIAPI *EFI_RESET_SYSTEM)(EFI_RESET_TYPE ResetType,EFI_STATUS ResetStatus,UINTN DataSize,VOID *ResetData);//プラットフォーム全体をリセットします。プラットフォームがサポートしている場合 (参照:EFI_RESET_NOTIFICATION_PROTOCOL)、プラットフォームのリセットを完了する前に、保留中の通知をすべて呼び出す必要があります。
typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT)(UINT32 *HighCount);//プラットフォームのモノトニック カウンターの次の上位 32 ビットを返します。
typedef EFI_STATUS(EFIAPI *EFI_UPDATE_CAPSULE)(EFI_CAPSULE_HEADER **CapsuleHeaderArray,UINTN CapsuleCount,EFI_PHYSICAL_ADDRESS ScatterGatherList);//仮想マッピングと物理マッピングの両方を使用して、カプセルをファームウェアに渡します。 意図された消費量に応じて、ファームウェアはカプセルを直ちに処理する場合があります。 ペイロードがシステム リセット後も保持される場合は、EFI_QueryCapsuleCapabilitiesから返されるリセット値をReset Systemに渡す必要があります。これにより、カプセルはリセット プロセスの一部としてファームウェアによって処理されます。
typedef EFI_STATUS(EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES)(EFI_CAPSULE_HEADER **CapsuleHeaderArray,UINTN CapsuleCount,UINT64 *MaximumCapsuleSize,EFI_RESET_TYPE *ResetType);//UpdateCapsule() を介してカプセルをサポートできるかどうかを返します。

typedef struct _EFI_CAPSULE_BLOCK_DESCRIPTOR{
	UINT64                           Length;
	union {
	   EFI_PHYSICAL_ADDRESS          DataBlock;
	   EFI_PHYSICAL_ADDRESS          ContinuationPointer;
	}Union;
} EFI_CAPSULE_BLOCK_DESCRIPTOR;

typedef struct _EFI_CAPSULE_HEADER{
	EFI_GUID             CapsuleGuid;
	UINT32               HeaderSize;
	UINT32               Flags;
	UINT32               CapsuleImageSize;
} EFI_CAPSULE_HEADER;

typedef struct _EFI_CAPSULE_TABLE{
	UINT32                  CapsuleArrayNumber;
	VOID*                   CapsulePtr[1];
} EFI_CAPSULE_TABLE;

typedef struct _EFI_MEMORY_RANGE{
	EFI_PHYSICAL_ADDRESS          Address;
	UINT64                        Length;
} EFI_MEMORY_RANGE;

typedef struct _EFI_MEMORY_RANGE_CAPSULE{
	EFI_CAPSULE_HEADER            Header;
	UINT32                        OsRequestedMemoryType;
	UINT64                        NumberOfMemoryRanges;
	EFI_MEMORY_RANGE              MemoryRanges[];
} EFI_MEMORY_RANGE_CAPSULE;

typedef struct _EFI_MEMORY_RANGE_CAPSULE_RESULT{
	UINT64               FirmwareMemoryRequirement;
	UINT64               NumberOfMemoryRanges;
} EFI_MEMORY_RANGE_CAPSULE_RESULT;

typedef struct _EFI_CAPSULE_RESULT_VARIABLE_HEADER{
	UINT32               VariableTotalSize;
	UINT32               Reserved; //for alignment
	EFI_GUID             CapsuleGuid;
	EFI_TIME             CapsuleProcessed;
	EFI_STATUS           CapsuleStatus;
}   EFI_CAPSULE_RESULT_VARIABLE_HEADER;

typedef struct _EFI_CAPSULE_RESULT_VARIABLE_FMP{
	UINT16               Version;
	UINT8                PayloadIndex;
	UINT8                UpdateImageIndex;
 
	EFI_GUID             UpdateImageTypeId;
	// CHAR16            CapsuleFileName [];
	// CHAR16            CapsuleTarget [];
}   EFI_CAPSULE_RESULT_VARIABLE_FMP;

typedef struct _EFI_CAPSULE_RESULT_VARIABLE_JSON{
	UINT32				 Version;
	UINT32				 CapsuleId;
	UINT32				 RespLength;
	UINT8				 Resp[];
} EFI_CAPSULE_RESULT_VARIABLE_JSON;