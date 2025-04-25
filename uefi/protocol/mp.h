#pragma once
#include <uefi/uefi_defined.h>

#define EFI_MP_SERVICES_PROTOCOL_GUID \
  {0x3fdda605,0xa76e,0x4f46,{0xad,0x29,0x12,0xf4,\
  0x53,0x1b,0x3d,0x08}}

//*******************************************************
// StatusFlag Bits Definition
//*******************************************************
#define PROCESSOR_AS_BSP_BIT        0x00000001
#define PROCESSOR_ENABLED_BIT       0x00000002
#define PROCESSOR_HEALTH_STATUS_BIT 0x00000004

//*******************************************************
// EFI_CPU_PHYSICAL_LOCATION
//*******************************************************
typedef struct {
	UINT32  Package;
	UINT32  Core;
	UINT32  Thread;
} EFI_CPU_PHYSICAL_LOCATION;

typedef struct {
	UINT32  Package;
	UINT32  Module;
	UINT32  Tile;
	UINT32  Die;
	UINT32  Core;
	UINT32  Thread;
} EFI_CPU_PHYSICAL_LOCATION2;

#define CPU_V2_EXTENDED_TOPOLOGY    BIT24
typedef union {
     EFI_CPU_PHYSICAL_LOCATION2     Location2;
} EXTENDED_PROCESSOR_INFORMATION;

//*******************************************************
// EFI_PROCESSOR_INFORMATION
//*******************************************************
typedef struct {
	UINT64                            ProcessorId;
	UINT32                            StatusFlag;
	EFI_CPU_PHYSICAL_LOCATION         Location;
	EXTENDED_PROCESSOR_INFORMATION    ExtendedInformation;
} EFI_PROCESSOR_INFORMATION;


typedef struct _EFI_MP_SERVICES_PROTOCOL EFI_MP_SERVICES_PROTOCOL; //MP サービス プロトコルをインストールすると、MP 管理に必要なサービスのコレクションが生成されます。

typedef VOID(EFIAPI *EFI_AP_PROCEDURE)(VOID *ProcedureArgument);
typedef EFI_STATUS(EFIAPI *EFI_MP_SERVICES_GET_NUMBER_OF_PROCESSORS)(EFI_MP_SERVICES_PROTOCOL *This,UINTN *NumberOfProcessors,UINTN *NumberOfEnabledProcessors);//このサービスは、プラットフォーム内の論理プロセッサの数と、現在有効な論理プロセッサの数を取得します。このサービスはBSPからのみ呼び出すことができます。
typedef EFI_STATUS(EFIAPI *EFI_MP_SERVICES_GET_PROCESSOR_INFO)(EFI_MP_SERVICES_PROTOCOL *This,UINTN *ProcessorNumber,EFI_PROCESSOR_INFORMATION *ProcessorInfoBuffer);//この呼び出しが行われた時点で、要求されたプロセッサの詳細なMP関連情報を取得します。このサービスはBSPからのみ呼び出すことができます。
typedef EFI_STATUS(EFIAPI *EFI_MP_SERVICES_STARTUP_ALL_APS)(EFI_MP_SERVICES_PROTOCOL*This,EFI_AP_PROCEDURE Procedure,BOOLEAN SingleThread,EFI_EVENT WaitEvent,UINTN TimeoutInMicroSeconds,VOID *ProcedureArgument,UINTN **FailedCpuList);//このサービスは、有効なすべてのAP上で、呼び出し元が指定した関数を実行します。APは同時に実行することも、1つずつ順番に実行することもできます。このサービスは、ブロッキングリクエストとノンブロッキングリクエストの両方をサポートします。ノンブロッキングリクエストはEFIイベントを使用するため、BSPはAPの終了を検出できます。詳細は、以下の「ノンブロッキング実行のサポート」を参照してください。このサービスはBSPからのみ呼び出すことができます。
typedef EFI_STATUS(EFIAPI *EFI_MP_SERVICES_STARTUP_THIS_AP)(EFI_MP_SERVICES_PROTOCOL *This,EFI_AP_PROCEDURE Procedure,UINTN ProcessorNumber,EFI_EVENT WaitEvent,UINTN TimeoutInMicroseconds,VOID *ProcedureArgument,BOOLEAN *Finished);//このサービスにより、呼び出し元は有効なAPを1つ取得し、呼び出し元が提供する関数を実行できます。呼び出し元は、BSPに対し、APの完了を待機するか、EFIイベントメカニズムを使用して次のタスクに進むかを指定できます。 EFI_MP_SERVICES_PROTOCOL.StartupAllAPs()詳細については、「非ブロッキング実行サポート」セクションを参照してください。このサービスはBSPからのみ呼び出すことができます。
typedef EFI_STATUS(EFIAPI *EFI_MP_SERVICES_SWITCH_BSP)(EFI_MP_SERVICES_PROTOCOL *This,UINTN ProcessorNumber,BOOLEAN EnableOldBSP);//このサービスは、要求されたAPをその時点以降のBSPに切り替えます。このサービスは、あらゆる目的においてBSPを変更します。このサービスは、現在のBSPからのみ呼び出すことができます。
typedef EFI_STATUS(EFIAPI *EFI_MP_SERVICES_ENABLEDISABLEAP)(EFI_MP_SERVICES_PROTOCOL *This,UINTN ProcessorNumber,BOOLEAN EnableAP,UINT32 *HealthFlag);//このサービスを使用すると、呼び出し元はこれ以降、AP を有効化または無効化できます。このサービスは BSP からのみ呼び出すことができます。
typedef EFI_STATUS(EFIAPI *EFI_MP_SERVICES_WHOAMI)(EFI_MP_SERVICES_PROTOCOL *This,UINTN *ProcessorNumber);//呼び出し元プロセッサのハンドル番号を返します。このサービスはBSPおよびAPから呼び出すことができます。


typedef struct _EFI_MP_SERVICES_PROTOCOL {
	EFI_MP_SERVICES_GET_NUMBER_OF_PROCESSORS  GetNumberOfProcessors;
	EFI_MP_SERVICES_GET_PROCESSOR_INFO        GetProcessorInfo;
	EFI_MP_SERVICES_STARTUP_ALL_APS           StartupAllAPs;
	EFI_MP_SERVICES_STARTUP_THIS_AP           StartupThisAP;
	EFI_MP_SERVICES_SWITCH_BSP                SwitchBSP;
	EFI_MP_SERVICES_ENABLEDISABLEAP           EnableDisableAP;
	EFI_MP_SERVICES_WHOAMI                    WhoAmI;
} EFI_MP_SERVICES_PROTOCOL;
  