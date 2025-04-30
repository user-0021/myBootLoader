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


void print_GUID(EFI_GUID guid){
	wcprintf(L"GUID:%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x\r\n\n"
		,guid.Data1,guid.Data2,guid.Data3
		,guid.Data4[0],guid.Data4[1],guid.Data4[2],guid.Data4[3],guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7]);
}

EFI_STATUS init_bootloader(CONST EFI_SYSTEM_TABLE *SystemTable,CONST EFI_HANDLE imageHandle,KERNEL_INFO* info){

	//Load Configuration Table
	wcprintf(L"\nLoad Configuration Table\r\n\n");

	UINTN i;
	for(i = 0;i < SystemTable->NumberOfTableEntries;i++){
		if(GUID_EQ(SystemTable->ConfigurationTable[i].VendorGuid,(EFI_GUID)EFI_ACPI_20_TABLE_GUID)){
			wcprintf(L"Tabletype:ACPI_20_TABLE\r\n");
			info->tables.acpi20Table = SystemTable->ConfigurationTable[i].VendorTable;
		}else if(GUID_EQ(SystemTable->ConfigurationTable[i].VendorGuid,(EFI_GUID)EFI_ACPI_TABLE_GUID)){
			wcprintf(L"Tabletype:ACPI_TABLE\r\n");
			info->tables.acpiTable = SystemTable->ConfigurationTable[i].VendorTable;
		}else if(GUID_EQ(SystemTable->ConfigurationTable[i].VendorGuid,(EFI_GUID)EFI_SMBIOS3_TABLE_GUID)){
			wcprintf(L"Tabletype:SMBIOS3_TABLE_GUID\r\n");
			info->tables.smBios3Table = SystemTable->ConfigurationTable[i].VendorTable;
		}else if(GUID_EQ(SystemTable->ConfigurationTable[i].VendorGuid,(EFI_GUID)EFI_SMBIOS_TABLE_GUID)){
			wcprintf(L"Tabletype:SMBIOS_TABLE_GUID\r\n");
			info->tables.smBiosTable = SystemTable->ConfigurationTable[i].VendorTable;
		}else{
			wcprintf(L"Tabletype:unknow\r\n");
		}
		print_GUID(SystemTable->ConfigurationTable[i].VendorGuid);
	}

	//check table
	if((info->tables.acpiTable == NULL && info->tables.acpi20Table == NULL) || (info->tables.smBiosTable == NULL && info->tables.smBios3Table == NULL)){
		return EFI_NOT_FOUND;
	}

	UINTN mapSize = 0;
	UINTN mapKey, descSize;
	UINT32 descVer;
	UINT8* descripterBuffer;

	//Get MemoryMap
	EFI_STATUS status = SystemTable->BootServices->GetMemoryMap(&mapSize,(void*)descripterBuffer,&mapKey,&descSize,&descVer);
	if(status == EFI_BUFFER_TOO_SMALL){
		mapSize = (mapSize & (~(UINTN)0xFFF)) + 0x2000;
		status = SystemTable->BootServices->AllocatePool(EfiBootServicesData,mapSize,(void*)&descripterBuffer);
		
		if(status == EFI_SUCCESS)
			status = SystemTable->BootServices->GetMemoryMap(&mapSize,(void*)descripterBuffer,&mapKey,&descSize,&descVer);
	}

	//Faile Check
	if(status == EFI_SUCCESS){
		//Exit Boot Service
		return SystemTable->BootServices->ExitBootServices(imageHandle,mapKey);
	}else{
		wcprintf(L"Failed GetMemoryMap: %0x\r\n",(UINT64)status);
		return status;
	}

	return EFI_NOT_READY;
}