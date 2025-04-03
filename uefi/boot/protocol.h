#pragma once
#include <uefi/uefi_defined.h>
#include <uefi/protocol/device.h>

//******************************************************
//EFI_OPEN_PROTOCOL_ATTERIBUTE
//******************************************************
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL   0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL         0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL        0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER  0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER            0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE            0x00000020

//******************************************************
//EFI_INTERFACE_TYPE
//******************************************************
typedef enum {
	EFI_NATIVE_INTERFACE
}EFI_INTERFACE_TYPE;

//******************************************************
// EFI_LOCATE_SEARCH_TYPE
//******************************************************
typedef enum {
	AllHandles,
	ByRegisterNotify,
	ByProtocol
}EFI_LOCATE_SEARCH_TYPE;


typedef struct _EFI_OPEN_PROTOCOL_INFORMATION_ENTRY EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;//


typedef EFI_STATUS(EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE)(EFI_HANDLE *Handle,EFI_GUID *Protocol,EFI_INTERFACE_TYPE InterfaceType,VOID *Interface);//デバイス ハンドルにプロトコル インターフェイスをインストールします。ハンドルが存在しない場合は、作成され、システムのハンドル リストに追加されます。InstallMultipleProtocolInterfaces() は InstallProtocolInterface() よりも多くのエラー チェックを実行するため、InstallProtocolInterface() の代わりに InstallMultipleProtocolInterfaces() を使用することをお勧めします。
typedef EFI_STATUS(EFIAPI *EFI_UNINSTALL_PROTOCOL_INTERFACE)(EFI_HANDLE Handle,EFI_GUID *Protocol,VOID *Interface);//デバイス ハンドルからプロトコル インターフェイスを削除します。UninstallProtocolInterface() の代わりに UninstallMultipleProtocolInterfaces() を使用することをお勧めします。
typedef EFI_STATUS(EFIAPI *EFI_REINSTALL_PROTOCOL_INTERFACE)(EFI_HANDLE Handle,EFI_GUID *Protocol,VOID *OldInterface,VOID *NewInterface);//デバイス ハンドルにプロトコル インターフェイスを再インストールします。
typedef EFI_STATUS(EFIAPI *EFI_REGISTER_PROTOCOL_NOTIFY)(EFI_GUID *Protocol,EFI_EVENT Event,VOID **Registration);//指定されたプロトコルのインターフェイスがインストールされるたびに通知されるイベントを作成します。
typedef EFI_STATUS(EFIAPI *EFI_LOCATE_HANDLE)(EFI_LOCATE_SEARCH_TYPE SearchType,EFI_GUID *Protocol,VOID *SearchKey,UINTN *BufferSize,EFI_HANDLE *Buffer);
typedef EFI_STATUS(EFIAPI *EFI_HANDLE_PROTOCOL)(EFI_HANDLE Handle,EFI_GUID *Protocol,VOID **Interface);//ハンドルを照会して、指定されたプロトコルをサポートしているかどうかを判断します。
typedef EFI_STATUS(EFIAPI *EFI_LOCATE_DEVICE_PATH)(EFI_GUID *Protocol,EFI_DEVICE_PATH_PROTOCOL **DevicePath,EFI_HANDLE *Device);//指定されたプロトコルをサポートするデバイス パス上のデバイスへのハンドルを検索します。
typedef EFI_STATUS(EFIAPI *EFI_OPEN_PROTOCOL)(EFI_HANDLE Handle,EFI_GUID *Protocol,VOID **Interface,EFI_HANDLE AgentHandle,EFI_HANDLE ControllerHandle,UINT32 Attributes);//ハンドルを照会して、指定されたプロトコルをサポートしているかどうかを判断します。プロトコルがハンドルでサポートされている場合は、呼び出しエージェントに代わってプロトコルを開きます。これは、EFI ブート サービス EFI_BOOT_SERVICES.HandleProtocol()の拡張バージョンです。
typedef EFI_STATUS(EFIAPI *EFI_CLOSE_PROTOCOL)(EFI_HANDLE Handle,EFI_GUID *Protocol,EFI_HANDLE AgentHandle,EFI_HANDLE ControllerHandle);//EFI_BOOT_SERVICES.OpenProtocol()を使用して開かれたハンドル上のプロトコルを閉じます 。
typedef EFI_STATUS(EFIAPI *EFI_OPEN_PROTOCOL_INFORMATION)(EFI_HANDLE Handle,EFI_GUID *Protocol,EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,UINTN *EntryCount);//現在プロトコル インターフェイスが開かれているエージェントのリストを取得します。
typedef EFI_STATUS(EFIAPI *EFI_CONNECT_CONTROLLER)(EFI_HANDLE ControllerHandle,EFI_HANDLE *DriverImageHandle,EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath,BOOLEAN Recursive);//1 つ以上のドライバーをコントローラーに接続します。
typedef EFI_STATUS(EFIAPI *EFI_DISCONNECT_CONTROLLER)(EFI_HANDLE ControllerHandle,EFI_HANDLE DriverImageHandle,EFI_HANDLE ChildHandle);//1 つ以上のドライバーをコントローラーから切断します。
typedef EFI_STATUS(EFIAPI *EFI_PROTOCOLS_PER_HANDLE)(EFI_HANDLE Handle,EFI_GUID ***ProtocolBuffer,UINTN *ProtocolBufferCount);//プールから割り当てられたバッファー内のハンドルにインストールされているプロトコル インターフェイス GUID のリストを取得します。
typedef EFI_STATUS(EFIAPI *EFI_LOCATE_HANDLE_BUFFER)(EFI_LOCATE_SEARCH_TYPE SearchType,EFI_GUID *Protocol,VOID *SearchKey,UINTN *NoHandles,EFI_HANDLE **Buffer);//プールから割り当てられたバッファ内の要求されたプロトコルをサポートするハンドルの配列を返します。
typedef EFI_STATUS(EFIAPI *EFI_LOCATE_PROTOCOL)(EFI_GUID *Protocol,VOID *Registration,VOID **Interface);//指定されたプロトコルに一致する最初のプロトコル インスタンスを返します。
typedef EFI_STATUS(EFIAPI *EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES)(EFI_HANDLE *Handle,...);//ブート サービス環境に 1 つ以上のプロトコル インターフェイスをインストールします。
typedef EFI_STATUS(EFIAPI *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES)(EFI_HANDLE Handle,...);//ブート サービス環境から 1 つ以上のプロトコル インターフェイスを削除します。



typedef struct _EFI_OPEN_PROTOCOL_INFORMATION_ENTRY{
	EFI_HANDLE                          AgentHandle;
	EFI_HANDLE                          ControllerHandle;
	UINT32                              Attributes;
	UINT32                              OpenCount;
}EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;