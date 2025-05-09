#include <pe-loader/loader.h>
#include <pe-loader/pe_type.h>
#include <uefi/lib/consoleio.h>
#include <mem/virtualAddress.h>

EFI_STATUS load_pe32(void* peFile,UINTN fileSize,void* mem,UINTN* memPages);
EFI_STATUS load_pe64(void* peFile,UINTN fileSize,void* mem,UINTN* memPages);
UINT32 pe_checksum(VOID* peFile,UINTN fileSize,VOID* checkSumPos);


EFI_STATUS load_pe(void* peFile,UINTN fileSize,void* mem,UINTN* memPages){
	//header 
	IMAGE_DOS_HEADER* dosHeader = peFile;
	IMAGE_NT_HEADERS32* ntHeader = (IMAGE_NT_HEADERS32*)(void*)((char*)peFile + dosHeader->e_lfanew);

	//check magic
	if(dosHeader->e_magic != IMAGE_DOS_HEAD_MAGIC || ntHeader->Signature != IMAGE_PE_HEAD_MAGIC)
		return EFI_UNSUPPORTED;

	//check PE or PE+
	if(ntHeader->OptionalHeader.Magic == IMAGE_OPTIONAL_HEADER_MAGIC32){
		return load_pe32(peFile,fileSize,mem,memPages);
	}else if(ntHeader->OptionalHeader.Magic == IMAGE_OPTIONAL_HEADER_MAGIC64){
		return load_pe64(peFile,fileSize,mem,memPages);
	}else{
		return EFI_UNSUPPORTED;
	}
}


EFI_STATUS load_pe32(void* peFile,UINTN fileSize,void* mem,UINTN* memPages){
	//header 
	IMAGE_DOS_HEADER* dosHeader = peFile;
	IMAGE_NT_HEADERS32* ntHeader = (IMAGE_NT_HEADERS32*)(void*)((char*)peFile + dosHeader->e_lfanew);

	//check arch
	#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	if(ntHeader->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
	#endif
		return EFI_UNSUPPORTED;

	//check characteristics
	UINT16 charactetistics = ntHeader->FileHeader.Characteristics;
	if((charactetistics & (IMAGE_FILE_RELOCS_STRIPPED | IMAGE_FILE_BYTES_REVERSED_LO | IMAGE_FILE_UP_SYSTEM_ONLY | IMAGE_FILE_DLL)) 
	|| !((charactetistics & (IMAGE_FILE_32BIT_MACHINE | IMAGE_FILE_EXECUTABLE_IMAGE)) == (IMAGE_FILE_32BIT_MACHINE | IMAGE_FILE_EXECUTABLE_IMAGE)))
		return EFI_UNSUPPORTED;

	//aligment check
	if(ntHeader->OptionalHeader.SectionAlignment != MEMORY_PAGE_SIZE)
		return EFI_UNSUPPORTED;

	//calc check sum
	UINT32 checkSum = pe_checksum(peFile,fileSize,&ntHeader->OptionalHeader.CheckSum);
	if(checkSum != ((UINT64)ntHeader->OptionalHeader.CheckSum))
		return EFI_UNSUPPORTED;
	

	//set number
	UINTN imageSize = dosHeader->e_cp;
	UINTN sectionCount = ntHeader->FileHeader.NumberOfSections;

	//IMAGE_SECTION_HEADER* sectionHeader = 

	

	return EFI_SUCCESS;
}


EFI_STATUS load_pe64(void* peFile,UINTN fileSize,void* mem,UINTN* memPages){

	return EFI_UNSUPPORTED;
	return EFI_SUCCESS;
}


UINT32 pe_checksum(VOID* peFile,UINTN fileSize,VOID* checkSumPos) {
	UINTN i = 0;
	UINT64 cSumBuf = 0;
	UINT32* cSumItr = peFile;
	UINT64 limit = (fileSize >> 2) + 1;
	for (i = 0;i < limit; i++)
	{
		if(cSumItr != checkSumPos){
			cSumBuf += *cSumItr;
			cSumBuf = (cSumBuf & 0xFFFFFFFF) + ((cSumBuf >> 32) & 0xFFFFFFFF);
		}
		cSumItr++;
	}
	cSumBuf = (cSumBuf&0xFFFF) + ((cSumBuf >> 16) & 0xFFFF);
	cSumBuf = (cSumBuf&0xFFFF) + ((cSumBuf >> 16) & 0xFFFF);
	cSumBuf += fileSize;

	return cSumBuf;
}