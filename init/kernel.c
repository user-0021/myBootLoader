#include <init/kernel.h>
#include <uefi/lib/consoleio.h>
#include <uefi/protocol/loaded_image.h>


#define OPEN_PROTOCOL(system,status,handle,protocol,interface,agentHandle) \
	status = system->BootServices->OpenProtocol(handle,&protocol,(void**)&interface,agentHandle,NULL,EFI_OPEN_PROTOCOL_GET_PROTOCOL);\
	if(status == EFI_SUCCESS)\
		s_wcprintf(system->ConOut,L"...Success\r\n");\
	else{\
		s_wcprintf(system->ConOut,L"...Failed(ErrorCode:%x)\r\n",(UINT64)status);\
		return status;\
	}

EFI_STATUS load_kernel(CONST EFI_SYSTEM_TABLE* systemTable,CONST EFI_HANDLE parent,CONST PROTOCOL_LIST* list,CONST CHAR16* kernel_file){

	s_wcprintf(systemTable->ConOut,L"\nTry load kernel\r\n");

	EFI_DEVICE_PATH_PROTOCOL* kernelPath	= list->devPathFromText->ConvertTextToDevicPath(kernel_file);
	EFI_DEVICE_PATH_PROTOCOL* completePath	= list->devPathUtilities->AppendDevicePath(list->devPath,kernelPath);
	s_wcprintf(systemTable->ConOut,L"Kernel path:%s\r\n",list->devPathToText->ConvertDevicePathToText(completePath,0,0));

	EFI_HANDLE kernelImage;
	EFI_STATUS status = systemTable->BootServices->LoadImage(FALSE,parent,completePath,NULL,0,&kernelImage);
	
	if(status == EFI_SUCCESS)
		s_wcprintf(systemTable->ConOut,L"...Success\r\n");\
	else{
		s_wcprintf(systemTable->ConOut,L"...Failed(ErrorCode:%x)\r\n",(UINT64)status);\
		return status;
	}

	s_wcprintf(systemTable->ConOut,L"\n\nTry loaded kernel infomation\r\n");
	EFI_GUID loadedImageProtocolGUID = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_LOADED_IMAGE_PROTOCOL* kernelLoadedImage;
	OPEN_PROTOCOL(systemTable,status,kernelImage,loadedImageProtocolGUID,kernelLoadedImage,parent);

	s_wcprintf(systemTable->ConOut,L"Kernel image base:%0x\r\n",(UINTN)kernelLoadedImage->ImageBase);
	s_wcprintf(systemTable->ConOut,L"Kernel image size:%0x\r\n",(UINTN)kernelLoadedImage->ImageSize);
	s_wcprintf(systemTable->ConOut,L"Kernel image code type:%0x\r\n",(UINTN)kernelLoadedImage->ImageCodeType);
	s_wcprintf(systemTable->ConOut,L"Kernel image data type:%0x\r\n",(UINTN)kernelLoadedImage->ImageDataType);



	return EFI_SUCCESS;
}