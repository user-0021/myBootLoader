#include <init/init.h>
#include <uefi/lib/consoleio.h>
#include <uefi/protocol/device.h>
#include <uefi/protocol/mp.h>


#define OPEN_PROTOCOL(system,status,handle,protocol,interface,agentHandle) \
	status = system->BootServices->OpenProtocol(handle,&protocol,(void**)&interface,agentHandle,NULL,EFI_OPEN_PROTOCOL_GET_PROTOCOL);\
	CHECK_SUCCSESS(status);

#define LOCATE_PROTOCOL(system,status,protocol,interface) \
	status = system->BootServices->LocateProtocol(&protocol,NULL,(void**)&interface);\
	CHECK_SUCCSESS(status);

#define HANDLE_PROTOCOL(system,status,handle,protocol,interface) \
	status = system->BootServices->HandleProtocol(handle,&protocol,(void**)&interface);\
	CHECK_SUCCSESS(status);


VOID EFIAPI ApHalt(__attribute__ ((__unused__)) VOID *Buffer) {
	while (1) {
		__asm__ __volatile__("hlt");
	}
}
	

EFI_STATUS init_protocol(CONST EFI_SYSTEM_TABLE* systemTable,CONST EFI_HANDLE imageHandle,BOOT_LOADER_DATA* data){
	//guid
	EFI_GUID devicePathGUID = EFI_DEVICE_PATH_PROTOCOL_GUID;
	EFI_GUID loadedImageGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_GUID devicePathToTextGUID = EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID;
	EFI_GUID devicePathFromTextGUID = EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;
	EFI_GUID devicePathUtilitiesGUID = EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID;
	EFI_GUID graphicsOutputGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GUID mpServicesGUID = EFI_MP_SERVICES_PROTOCOL_GUID;
	EFI_GUID simpleFileSystemGUID = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	
	//const value
	CHAR16 openMsg[] = L"Open %s\r\n";
	CHAR16 handleMsg[] = L"Handle %s\r\n";
	CHAR16 locateMsg[] = L"Locate %s\r\n";

	//Open protocol
	wcprintf(L"Try open protocols:\r\n");
	EFI_STATUS status;

	wcprintf(openMsg,L"EFI_LOADED_IMAGE_PROTOCOL");
	OPEN_PROTOCOL(systemTable,status,imageHandle,loadedImageGUID,data->protocols.bootLoaderImage,imageHandle);

	wcprintf(openMsg,L"EFI_DEVICE_PATH_PROTOCOL");
	OPEN_PROTOCOL(systemTable,status,data->protocols.bootLoaderImage->DeviceHandle,devicePathGUID,data->protocols.devPath,imageHandle);

	//Handle Protcol
	wcprintf(handleMsg,L"EFI_LOAD_FILE_PROTOCOL");
	HANDLE_PROTOCOL(systemTable,status,data->protocols.bootLoaderImage->DeviceHandle,simpleFileSystemGUID,data->protocols.simpleFileSystem);

	//Locate protocol
	wcprintf(L"\nTry find protocols:\r\n");
	
	wcprintf(locateMsg,L"EFI_DEVICE_PATH_TO_TEXT_PROTOCOL");
	LOCATE_PROTOCOL(systemTable,status,devicePathToTextGUID,data->protocols.devPathToText);

	wcprintf(locateMsg,L"EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL");
	LOCATE_PROTOCOL(systemTable,status,devicePathFromTextGUID,data->protocols.devPathFromText);

	wcprintf(locateMsg,L"EFI_DEVICE_PATH_UTILITIES_PROTOCOL");
	LOCATE_PROTOCOL(systemTable,status,devicePathUtilitiesGUID,data->protocols.devPathUtilities);

	wcprintf(locateMsg,L"EFI_GRAPHICS_OUTPUT_PROTOCOL");
	LOCATE_PROTOCOL(systemTable,status,graphicsOutputGUID,data->protocols.graphicsOut);
    
	EFI_MP_SERVICES_PROTOCOL *MpServices;
	wcprintf(locateMsg,L"EFI_MP_SERVICES_PROTOCOL");
	LOCATE_PROTOCOL(systemTable,status,mpServicesGUID,MpServices);

	//Open root
	wcprintf(L"Try open root volume");
	status = data->protocols.simpleFileSystem->OpenVolume(data->protocols.simpleFileSystem,&data->root);
	CHECK_SUCCSESS(status);
	
	//一旦他のコアは動作をさせない
    UINTN ProcessorCount;
    UINTN EnabledProcessorCount;

    // プロセッサ数を取得
	wcprintf(L"Try set mp func");
    status = MpServices->GetNumberOfProcessors(MpServices, &ProcessorCount, &EnabledProcessorCount);
	CHECK_SUCCSESS(status);

	UINTN i;
    for (i = 0; i < ProcessorCount; ++i) {
        BOOLEAN IsBSP;
        EFI_PROCESSOR_INFORMATION ProcInfo;
        status = MpServices->GetProcessorInfo(MpServices,& i, &ProcInfo);
        if (status != EFI_SUCCESS) continue;

        IsBSP = ProcInfo.StatusFlag & PROCESSOR_AS_BSP_BIT;
        if (!IsBSP) {
            // このAPをApHalt関数に移行させる
            status = MpServices->StartupThisAP(
                MpServices,
                ApHalt,
                i,
                NULL,     // WaitEvent
                0,        // Timeout in microseconds
                NULL,     // Procedure argument
                NULL      // Finished status
            );
        }
    }


	wcprintf(L"\n");

	return EFI_SUCCESS;
}