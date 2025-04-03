#include <kernel/mem/type.h>
#include <uefi/lib/consoleio.h>
#include <init/bootloader.h>

#define EFI_ACPI_20_TABLE_GUID \
  {0x8868e871,0xe4f1,0x11d3,\
    {0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}}



CHAR16* castTypeName(UINT16 type){

	switch (type)
	{
	case 0:
		return L"EfiReservedMemoryType";
	case 1:
		return L"EfiLoaderCode";
	case 2:
		return L"EfiLoaderData";
	case 3:
		return L"EfiBootServicesCode";
	case 4:
		return L"EfiBootServicesData";
	case 5:
		return L"EfiRuntimeServicesCode";
	case 6:
		return L"EfiRuntimeServicesData";
	case 7:
		return L"EfiConventionalMemory";
	case 8:
		return L"EfiUnusableMemory";
	case 9:
		return L"EfiACPIReclaimMemory";
	case 10:
		return L"EfiACPIMemoryNVS";
	case 11:
		return L"EfiMemoryMappedIO";
	case 12:
		return L"EfiMemoryMappedIOPortSpace";
	case 13:
		return L"EfiPalCode";
	case 14:
		return L"EfiPersistentMemory";
	case 15:
		return L"EfiUnacceptedMemoryType";
	case 16:
		return L"EfiMaxMemoryType";
	
	default:
		return L"NONE";
	}
}

EFI_STATUS init_bootloader(EFI_SYSTEM_TABLE *SystemTable,BOOTLOADER_DATA* data){

	//Load Configuration Table
	s_wcprintf(SystemTable->ConOut,L"Load Configuration Table\r\n");

	UINTN i;
	for(i = 0;i < SystemTable->NumberOfTableEntries;i++){
		s_wcprintf(SystemTable->ConOut
			,L"Load Entry\r\n"
			L"VendorGuid.Data1 : %08x\r\n"
			L"VendorGuid.Data2 : %04x\r\n"
			L"VendorGuid.Data3 : %04x\r\n"
			L"VendorGuid.Data4 : %02x %02x %02x %02x %02x %02x %02x %02x\r\n\n"
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data1
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data2
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data3
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data4[0]
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data4[1]
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data4[2]
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data4[3]
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data4[4]
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data4[5]
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data4[6]
			,(UINT64)SystemTable->ConfigurationTable[i].VendorGuid.Data4[7]
		);
	}

	return 1;

	UINTN mapSize = 2000;
	UINTN mapKey, descSize;
	UINT32 descVer;
	UINT8 descripterBuffer[2000];

	EFI_STATUS status = SystemTable->BootServices->GetMemoryMap(&mapSize,(void*)descripterBuffer,&mapKey,&descSize,&descVer);

	SystemTable->BootServices->Stall(1000000);

	if(status == EFI_SUCCESS){
		UINTN i;
		UINTN descCount = mapSize / descSize;
		EFI_MEMORY_DESCRIPTOR* desc = (void*)descripterBuffer;

		s_wcprintf(SystemTable->ConOut,L"MemoryMapSize         : 0x%x\r\n",mapSize);
		s_wcprintf(SystemTable->ConOut,L"MemoryDescripterSize  : 0x%x\r\n",descSize);
		s_wcprintf(SystemTable->ConOut,L"MemoryDescripterCount : 0x%x\r\n",descCount);

		

		for(i = 0;i < descCount;i++){

			s_wcprintf(SystemTable->ConOut,
				L"MemoryType     : %s\r\n"
				L"MemoryAddress  : 0x%x\r\n"
				L"MemoryVAddress : 0x%x\r\n"
				L"MemorySize     : 0x%x\r\n"
				L"Atteribute     : 0x%x\r\n"
				,castTypeName(desc->Type)
				,desc->PhysicalStart
				,desc->VirtualStart
				,desc->NumberOfPages
				,desc->Attribute);

				desc = (EFI_MEMORY_DESCRIPTOR*)(((UINT8*)desc) + descSize);
		}

	}
}