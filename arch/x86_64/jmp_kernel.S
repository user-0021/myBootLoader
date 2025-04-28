#include <mem/virtualAddress.h>

.section .text
.global jmp_kernel
.type jmp_kernel, @function

#define CR3_FLAG (0 & 0xFFF)

__jmp:
	hlt;
	jmp __jmp;

jmp_kernel:
	//Set CR3
	or CR3_FLAG, %rdx;
	mov %rdx, %cr3;

	//load gdt
	lgdt (%rcx);

	mov $0x10, %ax;
	mov %ax, %ds;
	mov %ax, %es;
	mov %ax, %ss;
	mov %ax, %fs;
	mov %ax, %gs;

	mov %r8,%rcx;
	mov $MEMORY_KERNEL_HEAD, %rax;
	add $MEMORY_PAGE_SIZE, %rax;
	pushq $0x08;
	pushq %rax;
	// jmp __jmp;
	lretq;

	// pushq $0x08;
	// lretq;


	mov $MEMORY_KERNEL_HEAD+MEMORY_PAGE_SIZE, %rax;
	jmp *%rax;
	ret;
