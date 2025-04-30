#include <init/bootloader.h>
#include <uefi/lib/consoleio.h>
#include <uefi/protocol/loaded_image.h>
#include <mem/virtualAddress.h>


#define OPEN_PROTOCOL(system,status,handle,protocol,interface,agentHandle) \
	status = system->BootServices->OpenProtocol(handle,&protocol,(void**)&interface,agentHandle,NULL,EFI_OPEN_PROTOCOL_GET_PROTOCOL);\
	if(status == EFI_SUCCESS)\
		wcprintf(L"...Success\r\n");\
	else{\
		wcprintf(L"...Failed(ErrorCode:0x%x)\r\n",(UINT64)status);\
		return status;\
	}

EFI_STATUS init_kernel(CONST EFI_SYSTEM_TABLE* systemTable,CONST EFI_HANDLE imageHandle,BOOT_LOADER_DATA* data,CONST CHAR16* kernel_file){
	wcprintf(L"\nTry load kernel\r\n");

	EFI_DEVICE_PATH_PROTOCOL* kernelPath	= data->protocols.devPathFromText->ConvertTextToDevicPath(kernel_file);
	EFI_DEVICE_PATH_PROTOCOL* completePath	= data->protocols.devPathUtilities->AppendDevicePath(data->protocols.devPath,kernelPath);
	wcprintf(L"Kernel path:%s\r\n",data->protocols.devPathToText->ConvertDevicePathToText(completePath,0,0));

	EFI_HANDLE kernelImage;
	EFI_STATUS status = systemTable->BootServices->LoadImage(FALSE,imageHandle,completePath,NULL,0,&kernelImage);
	
	if(status == EFI_SUCCESS)
		wcprintf(L"...Success\r\n");\
	else{
		wcprintf(L"...Failed(ErrorCode:0x%x)\r\n",(UINT64)status);\
		return status;
	}

	wcprintf(L"\n\nTry loaded kernel infomation\r\n");
	EFI_GUID loadedImageProtocolGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_LOADED_IMAGE_PROTOCOL* kernelLoadedImage;
	OPEN_PROTOCOL(systemTable,status,kernelImage,loadedImageProtocolGUID,kernelLoadedImage,imageHandle);

	wcprintf(L"\n");


	status = createVirtualMap(systemTable->BootServices,data->protocols.bootLoaderImage,kernelLoadedImage,data);
	if(status != EFI_SUCCESS){
		return EFI_NOT_READY;
	}

	return EFI_SUCCESS;
}