#include <pe-loader/loader.h>
#include <pe-loader/pe_type.h>
#include <uefi/lib/consoleio.h>
#include <mem/virtualAddress.h>

EFI_STATUS load_pe32(CONST EFI_SYSTEM_TABLE* systemTable,void* peFile,UINTN fileSize,BINARY_FILE* bf);
EFI_STATUS load_pe64(CONST EFI_SYSTEM_TABLE* systemTable,void* peFile,UINTN fileSize,BINARY_FILE* bf);
UINT32 pe_checksum(VOID* peFile,UINTN fileSize,VOID* checkSumPos);
UINT32 make_bf_flag(UINT32 flag);
UINT32 make_bs_flag(UINT32 flag);


EFI_STATUS load_pe(CONST EFI_SYSTEM_TABLE* systemTable,void* peFile,UINTN fileSize,BINARY_FILE* bf){
	//header 
	IMAGE_DOS_HEADER* dosHeader = peFile;
	IMAGE_NT_HEADERS32* ntHeader = (IMAGE_NT_HEADERS32*)(void*)((char*)peFile + dosHeader->e_lfanew);

	//check magic
	if(dosHeader->e_magic != IMAGE_DOS_HEAD_MAGIC || ntHeader->Signature != IMAGE_PE_HEAD_MAGIC)
		return EFI_UNSUPPORTED;

	//check PE or PE+
	if(ntHeader->OptionalHeader.Magic == IMAGE_OPTIONAL_HEADER_MAGIC32){
		return load_pe32(systemTable,peFile,fileSize,bf);
	}else if(ntHeader->OptionalHeader.Magic == IMAGE_OPTIONAL_HEADER_MAGIC64){
		return load_pe64(systemTable,peFile,fileSize,bf);
	}else{
		return EFI_UNSUPPORTED;
	}
}


EFI_STATUS load_pe32(CONST EFI_SYSTEM_TABLE* systemTable,void* peFile,UINTN fileSize,BINARY_FILE* bf){
	//header 
	IMAGE_DOS_HEADER* dosHeader = peFile;
	IMAGE_NT_HEADERS32* ntHeader = (IMAGE_NT_HEADERS32*)(void*)((char*)peFile + dosHeader->e_lfanew);
	IMAGE_SECTION_HEADER* sections = (IMAGE_SECTION_HEADER*)(void*)(ntHeader + 1);

	//check arch
	#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	if(ntHeader->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
	#endif
		return EFI_UNSUPPORTED;

	//aligment check
	if(ntHeader->OptionalHeader.SectionAlignment != MEMORY_PAGE_SIZE)
		return EFI_UNSUPPORTED;

	//calc check sum
	UINT32 checkSum = pe_checksum(peFile,fileSize,&ntHeader->OptionalHeader.CheckSum);
	if(checkSum != ((UINT64)ntHeader->OptionalHeader.CheckSum))
		return EFI_UNSUPPORTED;

	//set number
	UINTN sectionAligment = ntHeader->OptionalHeader.SectionAlignment;
	UINTN sectionAligmentMask = ntHeader->OptionalHeader.SectionAlignment - 1;

	//set bf
	bf->entry = ntHeader->OptionalHeader.AddressOfEntryPoint;
	bf->flag = make_bf_flag(ntHeader->FileHeader.Characteristics);
	bf->size = ntHeader->OptionalHeader.SizeOfImage;
	bf->base = ntHeader->OptionalHeader.ImageBase;
	bf->sectionCount = ntHeader->FileHeader.NumberOfSections;
	EFI_STATUS status = systemTable->BootServices->AllocatePool(EfiBootServicesData,bf->sectionCount * sizeof(BINARY_SECTION),(void**)&bf->sections);
	if(status != EFI_SUCCESS)
		return status;
	
	
	//set sections
	UINTN i;
	for(i = 0;i < bf->sectionCount;i++){
		bf->sections[i].base = sections[i].VirtualAddress;
		bf->sections[i].sectionSize = (sections[i].Misc.VirtualSize & (~sectionAligmentMask)) + sectionAligment*((sections[i].Misc.VirtualSize & sectionAligmentMask) != 0);
		bf->sections[i].imagePos = (EFI_PHYSICAL_ADDRESS)sections[i].PointerToRawData + (EFI_PHYSICAL_ADDRESS)peFile;
		bf->sections[i].imageSize = sections[i].SizeOfRawData;
		bf->sections[i].flag = make_bs_flag(sections[i].Characteristics);
	}

	return EFI_SUCCESS;
}


EFI_STATUS load_pe64(CONST EFI_SYSTEM_TABLE* systemTable,void* peFile,UINTN fileSize,BINARY_FILE* bf){
	//header 
	IMAGE_DOS_HEADER* dosHeader = peFile;
	IMAGE_NT_HEADERS64* ntHeader = (IMAGE_NT_HEADERS64*)(void*)((char*)peFile + dosHeader->e_lfanew);
	IMAGE_SECTION_HEADER* sections = (IMAGE_SECTION_HEADER*)(void*)(ntHeader + 1);

	//check arch
	#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	if(ntHeader->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
	#endif
		return EFI_UNSUPPORTED;

	//aligment check
	if(ntHeader->OptionalHeader.SectionAlignment != MEMORY_PAGE_SIZE)
		return EFI_UNSUPPORTED;

	//calc check sum
	UINT32 checkSum = pe_checksum(peFile,fileSize,&ntHeader->OptionalHeader.CheckSum);
	if(checkSum != ((UINT64)ntHeader->OptionalHeader.CheckSum))
		return EFI_UNSUPPORTED;

	//set number
	UINTN sectionAligment = ntHeader->OptionalHeader.SectionAlignment;
	UINTN sectionAligmentMask = ntHeader->OptionalHeader.SectionAlignment - 1;

	//set bf
	bf->entry = ntHeader->OptionalHeader.AddressOfEntryPoint;
	bf->flag = make_bf_flag(ntHeader->FileHeader.Characteristics);
	bf->size = ntHeader->OptionalHeader.SizeOfImage;
	bf->base = ntHeader->OptionalHeader.ImageBase;
	bf->sectionCount = ntHeader->FileHeader.NumberOfSections;
	EFI_STATUS status = systemTable->BootServices->AllocatePool(EfiBootServicesData,bf->sectionCount * sizeof(BINARY_SECTION),(void**)&bf->sections);
	if(status != EFI_SUCCESS)
		return status;
	
	
	//set sections
	UINTN i;
	for(i = 0;i < bf->sectionCount;i++){
		bf->sections[i].base = sections[i].VirtualAddress;
		bf->sections[i].sectionSize = (sections[i].Misc.VirtualSize & (~sectionAligmentMask)) + sectionAligment*((sections[i].Misc.VirtualSize & sectionAligmentMask) != 0);
		bf->sections[i].imagePos = (EFI_PHYSICAL_ADDRESS)sections[i].PointerToRawData + (EFI_PHYSICAL_ADDRESS)peFile;
		bf->sections[i].imageSize = sections[i].SizeOfRawData;
		bf->sections[i].flag = make_bs_flag(sections[i].Characteristics);
	}

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

UINT32 make_bf_flag(UINT32 flag){
	UINT32 res = 0;

	if(flag & IMAGE_FILE_EXECUTABLE_IMAGE)
		res |= BINARY_EXECUTEABLE;
	if(flag & IMAGE_FILE_32BIT_MACHINE)
		res |= BINARY_32BIT_IMAGE;
	if(!(flag & IMAGE_FILE_RELOCS_STRIPPED))
		res |= BINARY_RELOCATABLE;

	return res;
}


UINT32 make_bs_flag(UINT32 flag){
	UINT32 res = 0;

	if(flag & IMAGE_SCN_MEM_EXECUTE)
		res |= BINARY_SECTION_EXECUTE;
	if(!(flag & IMAGE_SCN_MEM_WRITE))
		res |= BINARY_SECTION_READONRY;

	return res;
}
