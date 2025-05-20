#include <init/init.h>
#include <uefi/lib/consoleio.h>
#include <uefi/protocol/loaded_image.h>
#include <uefi/protocol/media_access.h>
#include <mem/virtualAddress.h>
#include <pe-loader/loader.h>


#define OPEN_PROTOCOL(system,status,handle,protocol,interface,agentHandle) \
	status = system->BootServices->OpenProtocol(handle,&protocol,(void**)&interface,agentHandle,NULL,EFI_OPEN_PROTOCOL_GET_PROTOCOL);\
	CHECK_SUCCSESS(status);

EFI_STATUS load_kernel(CONST EFI_SYSTEM_TABLE* systemTable,CONST EFI_HANDLE imageHandle __attribute__ ((__unused__)),BOOT_LOADER_DATA* data,CHAR16* kernel_file){
	
	//open root dir
	wcprintf(L"\nTry open kernel file\r\n");
	EFI_FILE_PROTOCOL* kernelFile;
	EFI_STATUS status = data->root->Open(data->root,&kernelFile,kernel_file,EFI_FILE_MODE_READ,0);
	CHECK_SUCCSESS(status);

	//get file size
	wcprintf(L"\nTry get file size\r\n");
	EFI_GUID fileInfoID = EFI_FILE_INFO_ID;
	UINT8 infoBuffer[sizeof(EFI_FILE_INFO)<<1];
	UINTN fileInfoSize = sizeof(infoBuffer);
	status = kernelFile->GetInfo(kernelFile,&fileInfoID,&fileInfoSize,infoBuffer);
	CHECK_SUCCSESS(status);
	EFI_FILE_INFO* info = (VOID*)infoBuffer;

	//align 4KiB
	wcprintf(L"\nFile size is 0x%x [Byte]\r\n",info[0].FileSize);
	UINTN bufferSize = (info[0].FileSize & ~(0xFFF)) + 0x1000; 
	
	//load file
	wcprintf(L"\nTry allocate file buffer\r\n");
	EFI_PHYSICAL_ADDRESS buffer;
	status = systemTable->BootServices->AllocatePages(AllocateAnyPages,EfiBootServicesCode,bufferSize >> 12,&buffer);
	CHECK_SUCCSESS(status);
	systemTable->BootServices->SetMem((VOID*)buffer,bufferSize,0);

	wcprintf(L"\nTry read file\r\n");
	status = kernelFile->Read(kernelFile,&bufferSize,(void*)buffer);
	CHECK_SUCCSESS(status);
	
	wcprintf(L"\nTry load pe\r\n");
	BINARY_FILE kernelBinary;
	status = load_pe(systemTable,(void*)buffer,info[0].FileSize,&kernelBinary);
	CHECK_SUCCSESS(status);


	//create virtual memory map(page)
	status = createVirtualMap(systemTable->BootServices,data->protocols.bootLoaderImage,&kernelBinary,data);
	CHECK_SUCCSESS(status);

	return EFI_SUCCESS;
}