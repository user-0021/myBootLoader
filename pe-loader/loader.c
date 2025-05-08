#include <pe-loader/loader.h>
#include <pe-loader/pe_type.h>
#include <uefi/lib/consoleio.h>

EFI_STATUS load_pe32(CONST EFI_SYSTEM_TABLE *SystemTable,IMAGE_DOS_HEADER* dosHeader,VOID* dosStub,IMAGE_NT_HEADERS32* ntHeader);
EFI_STATUS load_pe64(CONST EFI_SYSTEM_TABLE *SystemTable,IMAGE_DOS_HEADER* dosHeader,VOID* dosStub,IMAGE_NT_HEADERS64* ntHeader);

EFI_STATUS load_pe(CONST EFI_SYSTEM_TABLE *SystemTable,void* buffer){
	//header 
	IMAGE_DOS_HEADER* dosHeader = buffer;
	VOID* dosStub = (VOID*)(dosHeader+1);
	IMAGE_NT_HEADERS32* ntHeader = buffer + dosHeader->e_lfanew;

	//check magic
	if(ntHeader->OptionalHeader.Magic == IMAGE_OPTIONAL_HEADER_MAGIC32){
		return load_pe32(SystemTable,dosHeader,dosStub,ntHeader);
	}else if(ntHeader->OptionalHeader.Magic == IMAGE_OPTIONAL_HEADER_MAGIC64){
		return load_pe64(SystemTable,dosHeader,dosStub,(void*)ntHeader);
	}else{
		return EFI_UNSUPPORTED;
	}
}


EFI_STATUS load_pe32(CONST EFI_SYSTEM_TABLE *SystemTable,IMAGE_DOS_HEADER* dosHeader,VOID* dosStub,IMAGE_NT_HEADERS32* ntHeader){

}


EFI_STATUS load_pe64(CONST EFI_SYSTEM_TABLE *SystemTable,IMAGE_DOS_HEADER* dosHeader,VOID* dosStub,IMAGE_NT_HEADERS64* ntHeader){

}
