#include <mem/virtualAddress.h>

.section .text
.global jmp_kernel
.type jmp_kernel, @function

#define CR3_FLAG (0 & 0xFFF)

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
	pushq $0x08;
	pushq $MEMORY_KERNEL_HEAD+MEMORY_PAGE_SIZE;
	lretq;

	pushq $0x08;
	// lretq;

	__jmp:
		hlt;
		jmp __jmp;

	mov $MEMORY_KERNEL_HEAD+MEMORY_PAGE_SIZE, %rax;
	jmp *%rax;
	ret;
