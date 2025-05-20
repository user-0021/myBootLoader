#pragma once
#include <uefi/uefi_defined.h>

//binary section flags
#define BINARY_SECTION_READONRY 0x00000001
#define BINARY_SECTION_EXECUTE  0x00000002

//binary file flag
#define BINARY_EXECUTEABLE 0x00000001
#define BINARY_RELOCATABLE 0x00000002
#define BINARY_32BIT_IMAGE 0x00000004


typedef struct _BINARY_SECTION{
	UINT32 flag;						//section flag
	UINTN sectionSize;					//section size
	EFI_PHYSICAL_ADDRESS base;			//section base
	UINTN imageSize;				 	//image size (in file size)
	EFI_PHYSICAL_ADDRESS imagePos;	//image offset (from file head)
} BINARY_SECTION;

typedef struct _BINARY_FILE {
	UINT32 flag;					//file flag
	UINTN size;						//binary size
	EFI_PHYSICAL_ADDRESS base;		//base address
	UINTN sectionCount;				//section count
	BINARY_SECTION* sections;		//section list
	EFI_PHYSICAL_ADDRESS entry;		//entry address
} BINARY_FILE;

EFI_STATUS load_pe(CONST EFI_SYSTEM_TABLE* systemTable,void* peFile,UINTN fileSize,BINARY_FILE* bf);

