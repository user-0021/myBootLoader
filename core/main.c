#include <uefi/uefi.h>
#include <uefi/lib/consoleio.h>
#include <init/bootloader.h>
#include <init/kernel.h>
#include <mem/virtualAddress.h>

#include "config.h"

#define KERNEL_PATH L"kernel/kernel.efi"


typedef struct{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) GDTPtr;

extern EFI_PHYSICAL_ADDRESS jmp_kernel(EFI_PHYSICAL_ADDRESS gdtptr,EFI_PHYSICAL_ADDRESS page4,BOOTLOADER_DATA* data);

inline void pain(BOOTLOADER_DATA* data){
	VOID *FrameBufferBase = (void*)data->protocols.graphicsOut->Mode->FrameBufferBase;
	UINT32 Width = data->data.Info.HorizontalResolution;
	UINT32 Height = data->data.Info.VerticalResolution;
	UINT32 PixelsPerScanLine = data->data.Info.PixelsPerScanLine;

	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *FB = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)FrameBufferBase;

	// 画面を真っ赤に塗る
	for (UINTN y = 0; y < Height; y++) {
		for (UINTN x = 0; x < Width; x++) {
			FB[y * PixelsPerScanLine + x].Red = 0;
			FB[y * PixelsPerScanLine + x].Green = 255;
			FB[y * PixelsPerScanLine + x].Blue = 0;
			FB[y * PixelsPerScanLine + x].Reserved = 0;
		}
	}
}

void nop(){
	while (1)
	{
		asm volatile ("hlt\n\t");
	}
	
}

typedef void(test)(BOOTLOADER_DATA* data);

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable){
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	stdout = SystemTable->ConOut;

	wcprintf(L"Success lunch bootloader\r\n\n");
	wcprintf(L"Load firmware info\r\n");
	wcprintf(L"FirmwareVender   : %s\r\n",SystemTable->FirmwareVendor);
	wcprintf(L"FirmwareRevision : 0x%0x\r\n\n",(UINT64)SystemTable->FirmwareRevision);


	//make gdt table pointer
	EFI_PHYSICAL_ADDRESS gdt;
	EFI_STATUS status = SystemTable->BootServices->AllocatePages(AllocateAnyPages,EfiLoaderData,1,&gdt);
	if(status != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*10);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	((long long *)gdt)[0] = 0x0000000000000000;
	((long long *)gdt)[1] = 0x00af9a000000ffff;
	((long long *)gdt)[2] = 0x00cf93000000ffff;

	GDTPtr gdtr = {
		.limit = (0x08 * 3) - 1,
		.base = gdt
	};

	BOOTLOADER_DATA data = {0};

	//init protpcol
	if(init_protocol(ImageHandle,SystemTable,&data.protocols) != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*3);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	//load kernel
	EFI_PHYSICAL_ADDRESS kernel_page4;
	if(load_kernel(SystemTable,ImageHandle,&data.protocols,KERNEL_PATH,&kernel_page4) != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*200);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	EFI_PHYSICAL_ADDRESS stack;
	status = SystemTable->BootServices->AllocatePages(AllocateAnyPages,EfiBootServicesData,5,&stack);
	stack += (0x5000 - 1) + MEMORY_DIRECTMAP_HEAD;

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
	UINTN SizeOfInfo;
	status = data.protocols.graphicsOut->QueryMode(data.protocols.graphicsOut, data.protocols.graphicsOut->Mode->Mode, &SizeOfInfo, &Info);
	data.data.Info = *Info;
	
	wcprintf(L"cr0: %0x\r\n",&data);
	wcprintf(L"cr2: %0x\r\n",data.protocols.bootLoaderImage->ImageBase);
	//init bootloader
	UINTN mapKey = 0;
	if(init_bootloader(SystemTable,&data,&mapKey) != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*3);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	/**********************
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 *   TOOOOOOOOOOOOOOOOOOOOOOOOOOOOODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
	 * 
	 *  実行ファイルの読み込みを自作する！ページのフラグ管理のため
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 * 
	 */


	//Exit Boot Service
	status = SystemTable->BootServices->ExitBootServices(ImageHandle,mapKey);
	if(status != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*3);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}
	
	asm volatile("mov %[page4], %%cr3\n\t"::[page4]"r"(kernel_page4));
	asm volatile("lgdt (%[ptgr])\n\t"::[ptgr]"r"(&gdtr));

	asm volatile("mov $0x10, %ax\n\t");
	asm volatile("mov %ax, %ds\n\t");
	asm volatile("mov %ax, %es\n\t");
	asm volatile("mov %ax, %ss\n\t");
	asm volatile("mov %ax, %fs\n\t");
	asm volatile("mov %ax, %gs\n\t");
	
	asm volatile("mov %[boot_data], %%rcx\n\t"::[boot_data]"r"(&data));
	asm volatile("mov %[ke], %%rax\n\t"::[ke]"r"(MEMORY_KERNEL_HEAD+MEMORY_PAGE_SIZE));

	asm volatile("pushq $0x08\n\t");
	asm volatile("pushq %rax\n\t");
	asm volatile("lretq\n\t");
	
	while (1)
	{
		asm volatile("hlt\n\t");
	}
	
	
	//RCX and RDX
	jmp_kernel((EFI_PHYSICAL_ADDRESS)&gdtr,kernel_page4,&data);

	//多分呼ばれない
	return EFI_SUCCESS;
}