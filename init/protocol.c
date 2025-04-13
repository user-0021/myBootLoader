#include <init/bootloader.h>
#include <uefi/lib/consoleio.h>
#include <uefi/protocol/device.h>


#define OPEN_PROTOCOL(system,status,handle,protocol,interface,agentHandle) \
	status = system->BootServices->OpenProtocol(handle,&protocol,(void**)&interface,agentHandle,NULL,EFI_OPEN_PROTOCOL_GET_PROTOCOL);\
	if(status == EFI_SUCCESS)\
		wcprintf(L"...Success\r\n");\
	else{\
		wcprintf(L"...Failed(ErrorCode:%x)\r\n",(UINT64)status);\
		return status;\
	}

#define LOCATE_PROTOCOL(system,status,protocol,interface) \
	status = system->BootServices->LocateProtocol(&protocol,NULL,(void**)&interface);\
	if(status == EFI_SUCCESS)\
		wcprintf(L"...Success\r\n");\
	else{\
		wcprintf(L"...Failed(ErrorCode:%x)\r\n",(UINT64)status);\
		return status;\
	}


EFI_STATUS init_protocol(CONST EFI_HANDLE imageHandle,CONST EFI_SYSTEM_TABLE* system,PROTOCOL_LIST* list){
	//const value
	CHAR16 openMsg[] = L"Open %s\r\n";
	CHAR16 locateMsg[] = L"Find %s\r\n";

	//Open protocol
	wcprintf(L"Try open protocols:\r\n");
	EFI_STATUS status;
	EFI_GUID devicePathGUID = EFI_DEVICE_PATH_PROTOCOL_GUID;
	EFI_GUID loadedImageGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;

	wcprintf(openMsg,L"EFI_LOADED_IMAGE_PROTOCOL");
	OPEN_PROTOCOL(system,status,imageHandle,loadedImageGUID,list->loadedImage,imageHandle);

	wcprintf(openMsg,L"EFI_DEVICE_PATH_PROTOCOL");
	OPEN_PROTOCOL(system,status,list->loadedImage->DeviceHandle,devicePathGUID,list->devPath,imageHandle);

	//Open protocol
	wcprintf(L"\nTry find protocols:\r\n");
	EFI_GUID devicePathToTextGUID = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
	EFI_GUID devicePathFromTextGUID = EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;
	EFI_GUID devicePathUtilitiesGUID = EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID;
	
	wcprintf(locateMsg,L"EFI_DEVICE_PATH_TO_TEXT_PROTOCOL");
	LOCATE_PROTOCOL(system,status,devicePathToTextGUID,list->devPathToText);

	wcprintf(locateMsg,L"EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL");
	LOCATE_PROTOCOL(system,status,devicePathFromTextGUID,list->devPathFromText);

	wcprintf(locateMsg,L"EFI_DEVICE_PATH_UTILITIES_PROTOCOL");
	LOCATE_PROTOCOL(system,status,devicePathUtilitiesGUID,list->devPathUtilities);

	wcprintf(L"\n");

	return EFI_SUCCESS;
}