#include <uefi/uefi_defined.h>
#include <init/init.h>
#include <arch/amd64/register.h>

VOID jmp_kernel(BOOT_LOADER_DATA* data,KERNEL_INFO* info){
	BOOT_LOADER_DATA* _data = data;
	KERNEL_INFO* _info = info;
	
	asm volatile(
	
		//load segmentation
		"lgdt (%[ptgr])\n\t"
		"mov $0x10, %%ax\n\t"
		"mov %%ax, %%ds\n\t"
		"mov %%ax, %%es\n\t"
		"mov %%ax, %%ss\n\t"
		"mov %%ax, %%fs\n\t"
		"mov %%ax, %%gs\n\t"

		//load page table
		"mov %[page4], %%cr3\n\t"

		//set argment
		"mov %[kernel_info], %%rcx\n\t"

		//jmp kernel code
		"mov %[kernel_entry], %%rax\n\t"
		

		//far jmp
		"pushq $0x08\n\t"
		"pushq %%rax\n\t"
		"lretq\n\t"


		:
		:[page4]"r"(_data->page4)
		,[ptgr]"r"(_data->gdtptr)
		,[kernel_info]"r"(_info)
		,[kernel_entry]"r"((UINT64)_data->kernelEntry)
		:"%rax","%rcx","%rdx"
	);
	
}