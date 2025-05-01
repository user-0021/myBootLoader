#include <uefi/uefi_defined.h>
#include <init/init.h>

#define CR0_FLAG 0x3B
#define CR4_FLAG 0x706C0
#define MSR_FLAG 0x901

VOID jmp_kernel(BOOT_LOADER_DATA* data,KERNEL_INFO* info){
	BOOT_LOADER_DATA* _data = data;
	KERNEL_INFO* _info = info;
	
	asm volatile(

		// SET CR4
		"mov %%cr4, %%rax\n\t"
		"or %[cr4_flag], %%rax\n\t"
		"mov %%rax, %%cr4\n\t"

		// Set LME (long mode enable)
		"mov $0xC0000080, %%rcx\n\t"
		"rdmsr\n\t"
		"or %[msr_flag],%%rax\n\t"
		"wrmsr\n\t"

		// Set CR0
		"mov %%cr0, %%rbx\n\t" 
		"or %[cr0_flag], %%rbx\n\t"
		"mov %%rbx, %%cr0\n\t" 

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
		:[cr0_flag]"r"((UINT64)CR0_FLAG)
		,[cr4_flag]"r"((UINT64)CR4_FLAG)
		,[msr_flag]"r"((UINT64)MSR_FLAG)
		,[page4]"r"(data->page4)
		,[ptgr]"r"(data->gdtptr)
		,[kernel_info]"r"(_info)
		,[kernel_entry]"r"((UINT64)_data->kernelEntry)
		:"%rax","%rcx"
	);
	
}