#include <mem/virtualAddress.h>

.section .text
.global jmp_kernel
.type jmp_kernel, @function

jmp_kernel:
	//RCX GDTR PTR
	//RDX PAGE4 ADDRESS
	lgdt (%rcx);

	mov 0x10, %ax;
	mov %ax, %ds;
	mov %ax, %es;
	mov %ax, %ss;
	mov %ax, %fs;
	mov %ax, %gs;
	
	//jmp 0x08, rdx;
	//pushq 0x08;
	//pushq %[kernel_start];
	//lretq;

	jmp 0x00;
