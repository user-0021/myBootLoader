#include <kernel/mem/type.h>
#include <uefi/lib/consoleio.h>
#include <init/bootloader.h>

#define EFI_ACPI_20_TABLE_GUID \
  {0x8868e871,0xe4f1,0x11d3,\
    {0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}}

#define EFI_ACPI_TABLE_GUID \
  {0xeb9d2d30,0x2d88,0x11d3,\
    {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

#define EFI_SAL_SYSTEM_TABLE_GUID \
  {0xeb9d2d32,0x2d88,0x11d3,\
   {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

#define EFI_SMBIOS_TABLE_GUID \
  {0xeb9d2d31,0x2d88,0x11d3,\
   {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

#define EFI_SMBIOS3_TABLE_GUID \
  {0xf2fd1544, 0x9794, 0x4a2c,\
   {0x99,0x2e,0xe5,0xbb,0xcf,0x20,0xe3,0x94}}

#define EFI_MPS_TABLE_GUID \
  {0xeb9d2d2f,0x2d88,0x11d3,\
   {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

#define GUID_EQ(left,right) (((left).Data1 == (right).Data1) && ((left).Data2 == (right).Data2) && ((left).Data3 == (right).Data3) &&\
   ((left).Data4[0] == (right).Data4[0]) && ((left).Data4[1] == (right).Data4[1]) && ((left).Data4[2] == (right).Data4[2]) && ((left).Data4[3] == (right).Data4[3]) &&\
   ((left).Data4[4] == (right).Data4[4]) && ((left).Data4[5] == (right).Data4[5]) && ((left).Data4[6] == (right).Data4[6]) && ((left).Data4[7] == (right).Data4[7]))



// #define	EFI_SMBIOS_SLOT_POPULATION_GUID = {03583ff6-cb36-4940-947e-b9b39f4afaf7}

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

void print_GUID(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut ,EFI_GUID guid){
	s_wcprintf(ConOut,
		L"GUID:%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x\r\n\n"
		,guid.Data1,guid.Data2,guid.Data3
		,guid.Data4[0],guid.Data4[1],guid.Data4[2],guid.Data4[3],guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7]);
}

EFI_STATUS init_bootloader(EFI_SYSTEM_TABLE *SystemTable,BOOTLOADER_DATA* data){

	//Load Configuration Table
	s_wcprintf(SystemTable->ConOut,L"\nLoad Configuration Table\r\n\n");

	UINTN i;
	for(i = 0;i < SystemTable->NumberOfTableEntries;i++){
		if(GUID_EQ(SystemTable->ConfigurationTable[i].VendorGuid,(EFI_GUID)EFI_ACPI_20_TABLE_GUID)){
			s_wcprintf(SystemTable->ConOut,L"Tabletype:ACPI_20_TABLE\r\n");
			data->tables.acpi20Table = SystemTable->ConfigurationTable[i].VendorTable;
		}else if(GUID_EQ(SystemTable->ConfigurationTable[i].VendorGuid,(EFI_GUID)EFI_ACPI_TABLE_GUID)){
			s_wcprintf(SystemTable->ConOut,L"Tabletype:ACPI_TABLE\r\n");
			data->tables.acpiTable = SystemTable->ConfigurationTable[i].VendorTable;
		}else if(GUID_EQ(SystemTable->ConfigurationTable[i].VendorGuid,(EFI_GUID)EFI_SMBIOS3_TABLE_GUID)){
			s_wcprintf(SystemTable->ConOut,L"Tabletype:SMBIOS3_TABLE_GUID\r\n");
			data->tables.smBios3Table = SystemTable->ConfigurationTable[i].VendorTable;
		}else if(GUID_EQ(SystemTable->ConfigurationTable[i].VendorGuid,(EFI_GUID)EFI_SMBIOS_TABLE_GUID)){
			s_wcprintf(SystemTable->ConOut,L"Tabletype:SMBIOS_TABLE_GUID\r\n");
			data->tables.smBiosTable = SystemTable->ConfigurationTable[i].VendorTable;
		}else{
			s_wcprintf(SystemTable->ConOut,L"Tabletype:unknow\r\n");
		}
		print_GUID(SystemTable->ConOut,SystemTable->ConfigurationTable[i].VendorGuid);
	}

	//check table
	if((data->tables.acpiTable == NULL && data->tables.acpi20Table == NULL) || (data->tables.smBiosTable == NULL && data->tables.smBios3Table == NULL)){
		return EFI_NOT_FOUND;
	}

	UINTN mapSize = 0;
	UINTN mapKey, descSize;
	UINT32 descVer;
	UINT8* descripterBuffer;

	EFI_STATUS status = SystemTable->BootServices->GetMemoryMap(&mapSize,(void*)descripterBuffer,&mapKey,&descSize,&descVer);
	if(status == EFI_BUFFER_TOO_SMALL){
		mapSize = (mapSize & (~(UINTN)0xFFF)) + 0x2000;
		status = SystemTable->BootServices->AllocatePool(EfiBootServicesData,mapSize,(void*)&descripterBuffer);
		
		if(status == EFI_SUCCESS)
			status = SystemTable->BootServices->GetMemoryMap(&mapSize,(void*)descripterBuffer,&mapKey,&descSize,&descVer);
	}

	if(status == EFI_SUCCESS){
		UINTN i;
		UINTN descCount = mapSize / descSize;
		EFI_MEMORY_DESCRIPTOR* desc = (void*)descripterBuffer;

		s_wcprintf(SystemTable->ConOut,L"MemoryMapSize         : 0x%x\r\n",mapSize);
		s_wcprintf(SystemTable->ConOut,L"MemoryDescripterSize  : 0x%x\r\n",descSize);
		s_wcprintf(SystemTable->ConOut,L"MemoryDescripterCount : 0x%x\r\n\n",descCount);

		

		for(i = 0;i < descCount;i++){

			s_wcprintf(SystemTable->ConOut,
				L"MemoryType     : %s\r\n"
				L"MemoryAddress  : 0x%x\r\n"
				L"MemoryVAddress : 0x%x\r\n"
				L"MemorySize     : 0x%x\r\n"
				L"Atteribute     : 0x%x\r\n\n"
				,castTypeName(desc->Type)
				,desc->PhysicalStart
				,desc->VirtualStart
				,desc->NumberOfPages
				,desc->Attribute);

				desc = (EFI_MEMORY_DESCRIPTOR*)(((UINT8*)desc) + descSize);
		}

	}else{
		return status;
	}

	return EFI_SUCCESS;
}