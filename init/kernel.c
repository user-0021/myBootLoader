#include <init/kernel.h>
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

EFI_STATUS load_kernel(CONST EFI_SYSTEM_TABLE* systemTable,CONST EFI_HANDLE parent,CONST PROTOCOL_LIST* list,CONST CHAR16* kernel_file,EFI_PHYSICAL_ADDRESS* kernelPage4){
	wcprintf(L"\nTry load kernel\r\n");

	EFI_DEVICE_PATH_PROTOCOL* kernelPath	= list->devPathFromText->ConvertTextToDevicPath(kernel_file);
	EFI_DEVICE_PATH_PROTOCOL* completePath	= list->devPathUtilities->AppendDevicePath(list->devPath,kernelPath);
	wcprintf(L"Kernel path:%s\r\n",list->devPathToText->ConvertDevicePathToText(completePath,0,0));

	EFI_HANDLE kernelImage;
	EFI_STATUS status = systemTable->BootServices->LoadImage(FALSE,parent,completePath,NULL,0,&kernelImage);
	
	if(status == EFI_SUCCESS)
		wcprintf(L"...Success\r\n");\
	else{
		wcprintf(L"...Failed(ErrorCode:0x%x)\r\n",(UINT64)status);\
		return status;
	}

	wcprintf(L"\n\nTry loaded kernel infomation\r\n");
	EFI_GUID loadedImageProtocolGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_LOADED_IMAGE_PROTOCOL* kernelLoadedImage;
	OPEN_PROTOCOL(systemTable,status,kernelImage,loadedImageProtocolGUID,kernelLoadedImage,parent);

	wcprintf(L"Kernel image base:0x%0x\r\n",(UINTN)kernelLoadedImage->ImageBase);
	wcprintf(L"Kernel image size:0x%0x\r\n",(UINTN)kernelLoadedImage->ImageSize);
	wcprintf(L"Kernel image code type:0x%0x\r\n",(UINTN)kernelLoadedImage->ImageCodeType);
	wcprintf(L"Kernel image data type:0x%0x\r\n",(UINTN)kernelLoadedImage->ImageDataType);

	wcprintf(L"\n");


	EFI_PHYSICAL_ADDRESS pointer = createVirtualMap(systemTable->BootServices,list->bootLoaderImage,kernelLoadedImage);
	if(pointer == (EFI_PHYSICAL_ADDRESS)NULL){
		return EFI_NOT_READY;
	}
	*kernelPage4 = pointer;

	return EFI_SUCCESS;
}