#include <arch/x86_64/virtualAddress.h>

#define PAGE_SIZE 0x1000

#define ENTRY_PRESENT(b) 				((b & 1UL) << 0)
#define ENTRY_WRITABLE(b) 				((b & 1UL) << 1)
#define ENTRY_USER_ACCESSIBLE(b) 		((b & 1UL) << 2)
#define ENTRY_WRITE_THROUGH_CACHING(b) 	((b & 1UL) << 3)
#define ENTRY_DISABLE_CACHE(b)			((b & 1UL) << 4)
#define ENTRY_ACCESSED(b) 				((b & 1UL) << 5)
#define ENTRY_DIRTY(b) 					((b & 1UL) << 6)
#define ENTRY_HUGE_PAGE(b) 				((b & 1UL) << 7)
#define ENTRY_GROABAL(b) 				((b & 1UL) << 8)
#define ENTRY_LAVAILEBLE(data)			((data & 7UL) << 9)
#define ENTRY_PHYSICAL_ADDRESS(address)	((address & 0xFFFFFFFFFFUL) << 12)
#define ENTRY_HAVAILEBLE(data)			((data & 0x7FUL) << 52)
#define ENTRY_EXECUTEBLE(b)				((b & 0x1UL) << 63)

EFI_PHYSICAL_ADDRESS* createVirtualMap(EFI_BOOT_SERVICES* bootService,EFI_LOADED_IMAGE_PROTOCOL* kernel,EFI_VIRTUAL_ADDRESS* preaceAddress){
	EFI_PHYSICAL_ADDRESS* lev4_page = bootService->AllocatePages(AllocateAnyPages,EfiLoaderData,1,NULL);
	bootService->SetMem(lev4_page,PAGE_SIZE,0);
	
}