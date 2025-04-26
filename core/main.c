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
		.base = gdt + MEMORY_DIRECTMAP_HEAD
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


	// UINT64 cr0,cr2,cr3,cr4;
	// asm volatile ("mov %%cr0, %[reg]\n\t" : [reg]"=r"(cr0));
	// asm volatile ("mov %%cr2, %[reg]\n\t" : [reg]"=r"(cr2));
	// asm volatile ("mov %%cr3, %[reg]\n\t" : [reg]"=r"(cr3));
	// asm volatile ("mov %%cr4, %[reg]\n\t" : [reg]"=r"(cr4));

	// wcprintf(L"cr0: %0x\r\n",cr0);
	// wcprintf(L"cr2: %0x\r\n",cr2);
	// wcprintf(L"cr3: %0x\r\n",cr3);
	// wcprintf(L"cr4: %0x\r\n",cr4);

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
	UINTN SizeOfInfo;
	status = data.protocols.graphicsOut->QueryMode(data.protocols.graphicsOut, data.protocols.graphicsOut->Mode->Mode, &SizeOfInfo, &Info);
	data.data.Info = *Info;

	// UINT64 i;
	// for(i = 0;i < 512;i++){
	// 	EFI_PHYSICAL_ADDRESS page3 = (*(UINT64*)(void*)(kernel_page4 + (i << 3))) & 0xFFFFFFFFFF000UL;
		
	// 	if(page3 != 0){
	// 		UINT64 j;
	// 		for(j = 0;j < 512;j++){
	// 			EFI_PHYSICAL_ADDRESS page2 = (*(UINT64*)(void*)(page3 + (j << 3))) & 0xFFFFFFFFFF000UL;
				
	// 			if(page2 != 0){

	// 				UINT64 k;
	// 				for(k = 0;k < 512;k++){
	// 					EFI_PHYSICAL_ADDRESS page1 = (*(UINT64*)(void*)(page2 + (k << 3))) & 0xFFFFFFFFFF000UL;
						
	// 					if(page1 != 0){
							
	// 						UINT64 l;
	// 						for(l = 0;l < 512;l++){
	// 							EFI_PHYSICAL_ADDRESS page = (*(UINT64*)(void*)(page1 + (l << 3))) & 0xFFFFFFFFFF000UL;
								
	// 							if(page != 0){
									
	// 								UINT64 m = 0;
	// 								// for(m = 0;m < 512;m++){//(i << 39) | (j << 30) | (k << 21) | (l << 12) | (m << 3)
	// 									wcprintf(L"0x%0x : 0x%0x\r\n",(i << 39) | (j << 30) | (k << 21) | (l << 12) | (m << 3),page);
	// 								// }
	// 							}
	// 						}
	// 					}
	// 				}
					
	// 			}
	// 		}
	// 	}
	// }

	//Exit Boot Service
	status = SystemTable->BootServices->ExitBootServices(ImageHandle,mapKey);
	if(status != EFI_SUCCESS){
		SystemTable->BootServices->Stall(1000*1000*3);
		SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown,EFI_SUCCESS,0,NULL);
	}

	jmp_kernel((EFI_PHYSICAL_ADDRESS)&gdtr,kernel_page4,&data);
	
	asm volatile("mov %[page4], %%cr3\n\t"::[page4]"r"(kernel_page4));
	asm volatile("lgdt (%[ptgr])\n\t"::[ptgr]"r"(&gdtr));

	asm volatile("mov $0x10, %ax\n\t");
	asm volatile("mov %ax, %ds\n\t");
	asm volatile("mov %ax, %es\n\t");
	asm volatile("mov %ax, %ss\n\t");
	asm volatile("mov %ax, %fs\n\t");
	asm volatile("mov %ax, %gs\n\t");
	EFI_PHYSICAL_ADDRESS b_data = (EFI_PHYSICAL_ADDRESS)((void*)(&data));
	b_data += MEMORY_DIRECTMAP_HEAD;
	// data.protocols.graphicsOut->Mode->FrameBufferBase += MEMORY_DIRECTMAP_HEAD;
	data.protocols.graphicsOut->Mode = ((void*)data.protocols.graphicsOut->Mode) + MEMORY_DIRECTMAP_HEAD;
	data.protocols.graphicsOut = ((void*)data.protocols.graphicsOut) + MEMORY_DIRECTMAP_HEAD;
	
	asm volatile("mov %[kernel_head], %%rsp\n\t"::[kernel_head]"r"(stack));
	asm volatile("mov %[boot_data], %%rcx\n\t"::[boot_data]"r"(b_data));

	asm volatile("pushq $0x08\n\t");
	asm volatile("pushq %[kernel_entry]\n\t"::[kernel_entry]"r"(MEMORY_KERNEL_HEAD+MEMORY_PAGE_SIZE));
	asm volatile("lretq\n\t");
	

	VOID *FrameBufferBase = (void*)data.protocols.graphicsOut->Mode->FrameBufferBase;
	UINT32 Width = data.data.Info.HorizontalResolution;
	UINT32 Height = data.data.Info.VerticalResolution;
	UINT32 PixelsPerScanLine = data.data.Info.PixelsPerScanLine;

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


	while (1)
	{
		asm volatile("hlt\n\t");
	}
	
	
	//RCX and RDX
	jmp_kernel((EFI_PHYSICAL_ADDRESS)&gdtr,kernel_page4,&data);

	//多分呼ばれない
	return EFI_SUCCESS;
}