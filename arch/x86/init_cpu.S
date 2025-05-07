.section .text
.code32
.global init_cpu
.type init_cpu,@function

init_cpu:
	//push arg
	push %ecx;

	//disable paging and begine 32bit mode
	mov %cr0, %ebx;
	and ~(1 << 31), %ebx;
	mov %ebx, %cr0;

	// SET CR4
	mov 0x706E0, %eax;
	mov %eax, %cr4;

	// Set LME (long mode enable)
	mov 0xC0000080, %ecx;
	mov 0x901,%eax;
	wrmsr

	//set page4 address
	pop %ecx;
	mov %ecx, %cr3;

	// Set CR0
	mov 0xB, %ebx;
	mov %ebx, %cr0; 
	
	lret;