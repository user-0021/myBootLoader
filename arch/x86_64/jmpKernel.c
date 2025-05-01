#include <uefi/uefi_defined.h>
#include <init/init.h>

VOID jmp_kernel(BOOT_LOADER_DATA* data,KERNEL_INFO* info){
	
	asm volatile(

		//load page table
		"mov %[page4], %%cr3\n\t"
		
		//load segmentation
		"lgdt (%[ptgr])\n\t"
		"mov $0x10, %%ax\n\t"
		"mov %%ax, %%ds\n\t"
		"mov %%ax, %%es\n\t"
		"mov %%ax, %%ss\n\t"
		"mov %%ax, %%fs\n\t"
		"mov %%ax, %%gs\n\t"

		//load data
		"mov %[kernel_info], %%rcx\n\t"
		"mov %[kernel_entry], %%rax\n\t"

		//far jmp
		"pushq $0x08\n\t"
		"pushq %%rax\n\t"
		"lretq\n\t"

		:
		:[page4]"r"(data->page4)
		,[ptgr]"r"(data->gdtptr)
		,[kernel_info]"r"(info)
		,[kernel_entry]"r"((UINT64)data->kernelEntry)
		:"%rax","%rcx"
	);
	
}