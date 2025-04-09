#include <arch/x86_64/virtualAddress.h>

EFI_PHYSICAL_ADDRESS* createVirtualMap(EFI_ALLOCATE_PAGES allocatePage,EFI_LOADED_IMAGE_PROTOCOL* kernel,EFI_VIRTUAL_ADDRESS* preaceAddress){
	EFI_PHYSICAL_ADDRESS* lev4_page = allocatePage(AllocateAnyPages,EfiLoaderData,1,NULL);

}