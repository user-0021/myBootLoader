OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(efi_main)
SECTIONS
{
	. = 0x1000;
	ImageBase = .;
	.hash : { *(.hash) }
	.gnu.hash : { *(.gnu.hash) }
	. = ALIGN(0x1000);
	.text :
	{
		_text = .;
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t.*)
		. = ALIGN(16);
	}
	_etext = .;
	_text_size = . - _text;
	. = ALIGN(0x1000);
	.rdata :
	{
		_data = .;
		*(.got.plt)
		*(.got)
		*(.rodata*)
		*(.srodata)
		*(.gnu.linkonce.r.*)
	}
	. = ALIGN(0x1000);
	.data :
	{
		*(.data*)
		*(.sdata*)
		*(.gnu.linkonce.d.*)
	}
	. = ALIGN(0x1000);
	.bss :
	{
		*(.sbss)
		*(.scommon)
		*(.dynbss)
		*(.bss)
		*(.gnu.linkonce.b.*)
		*(COMMON)
	}
	. = ALIGN(0x1000);
	.dynamic  : { *(.dynamic) }
	. = ALIGN(0x1000);
	.rel :
	{
		*(.rel.data)
		*(.rel.data.*)
		*(.rel.got)
		*(.rel.stab)
	}
	_edata = .;
	_data_size = . - _etext;
	. = ALIGN(0x1000);
	.reloc :
	{
		LONG(_data);
		LONG(10);
		SHORT(0);
		*(.reloc)
	}
	. = ALIGN(0x1000);
	.dynsym   : { *(.dynsym) }
	. = ALIGN(0x1000);
	.dynstr   : { *(.dynstr) }
	. = ALIGN(0x1000);
	/DISCARD/ :
	{
		*(.rel.reloc)
		*(.eh_frame)
		*(.note*)
		*(.comment*)
	}
	_kernel_end = .;
}