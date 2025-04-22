#include <mem/virtualAddress.h>

.section .text
.global jmp_kernel
.type jmp_kernel, @function

#define CR3_FLAG (0 & 0xFFF)

jmp_kernel:

	//RCX GDTR PTR
	//RDX PAGE4 ADDRESS
	// pushq %rcx;
	// pushq %rdx;

	// jmp 0x00 ;
	//disable paging
	// mov cr0, %rbx;
	// mov %cr0, %rbx;
	// mov %cr0, %rax;
	// and $0x7FFFFFFF, %rbx;
	// mov %rbx, %cr0;

	// //move page4 address
	// mov %edx, %eax;

	// //Set LME (long mode enable) 
	// //Set NXE (Execute Disable bit enable)
	// mov $0xC0000080, %ecx;
	// rdmsr;
	// or MSR_FLAG, %eax;
	// wrmsr;


	// //Enable PAE
	// mov %cr4, %edx;
	// or  CR4_FLAG, %edx;
	// mov %edx, %cr4;

	//Set CR3
	or CR3_FLAG, %rdx;
	mov %rdx, %cr3;

	//Enable paging (and protected mode, if it isn't already active)
	// mov %cr0, %rbx;
	// or $0x80000800, %ebx;
	// mov %rax, %cr0;




	// popq %rdx;
	// popq %rcx;

	lgdt (%rcx);

	mov $0x10, %ax;
	mov %ax, %ds;
	mov %ax, %es;
	mov %ax, %ss;
	mov %ax, %fs;
	mov %ax, %gs;

	pushq $0x08;
	pushq $MEMORY_KERNEL_HEAD+MEMORY_PAGE_SIZE;
	lretq;
	// mov $MEMORY_KERNEL_HEAD+MEMORY_PAGE_SIZE, %rax;
	// jmp %rax;

label:
	jmp label;