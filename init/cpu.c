#include <init/init.h>
#include <uefi/lib/consoleio.h>
#include <uefi/protocol/media_access.h>
#include <pe-loader/loader.h>



EFI_STATUS load_cpu(CONST EFI_SYSTEM_TABLE* systemTable,CONST EFI_HANDLE imageHandle,BOOT_LOADER_DATA* data){
	wcprintf(L"\nTry load init_cpu\r\n");

	//open root dir
	wcprintf(L"\nTry open file\r\n");
	EFI_FILE_PROTOCOL* initCPU;
	EFI_STATUS status = data->root->Open(data->root,&initCPU,INIT_CPU_IMAGE_PATH,EFI_FILE_MODE_READ,0);
	CHECK_SUCCSESS(status);

	//get file size
	wcprintf(L"\nTry get file size\r\n");
	EFI_GUID fileInfoID = EFI_FILE_INFO_ID;
	EFI_FILE_INFO info[2];
	UINTN fileInfoSize = sizeof(info);
	status = initCPU->GetInfo(initCPU,&fileInfoID,&fileInfoSize,info);
	CHECK_SUCCSESS(status);

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
	status = initCPU->Read(initCPU,&bufferSize,(void*)buffer);
	CHECK_SUCCSESS(status);
	
	wcprintf(L"\nTry load pe\r\n");
	UINTN imageSize = 0;
	status = load_pe((void*)buffer,bufferSize,NULL,&imageSize);
	CHECK_SUCCSESS(status);

	while (1)
	{
		/* code */
	}
	

	return EFI_SUCCESS;
}