#build executable file
build:bootLoader.elf

#make objs
obj:
	@mkdir obj

obj/main.o: core/main.c
	x86_64-elf-gcc -o obj/main.o core/main.c -I . -ffreestanding -fpic -fno-stack-protector -fshort-wchar -mno-red-zone -mgeneral-regs-only -mabi=ms -Wall -Wextra -Wpedantic -O3 -c

obj/bootloader.o: init/bootloader.c
	x86_64-elf-gcc -o obj/bootloader.o init/bootloader.c -I . -ffreestanding -fpic -fno-stack-protector -fshort-wchar -mno-red-zone -mgeneral-regs-only -mabi=ms -Wall -Wextra -Wpedantic -O3 -c

obj/protocol.o: init/protocol.c
	x86_64-elf-gcc -o obj/protocol.o init/protocol.c -I . -ffreestanding -fpic -fno-stack-protector -fshort-wchar -mno-red-zone -mgeneral-regs-only -mabi=ms -Wall -Wextra -Wpedantic -O3 -c

obj/consoleio.o: uefi/lib/consoleio.c
	x86_64-elf-gcc -o obj/consoleio.o uefi/lib/consoleio.c -I . -ffreestanding -fpic -fno-stack-protector -fshort-wchar -mno-red-zone -mgeneral-regs-only -mabi=ms -Wall -Wextra -Wpedantic -O3 -c

bootLoader.elf: obj obj/main.o obj/bootloader.o obj/protocol.o obj/consoleio.o
	x86_64-elf-gcc -o bootLoader.elf obj/main.o obj/bootloader.o obj/protocol.o obj/consoleio.o -nostdlib -shared -Wl,-T,x86_64.lds -Wl,-Bsymbolic -Wl,-znocombreloc -lgcc

all: clean bootLoader.elf

clean:
	$(RM) bootLoader.elf obj/main.o obj/bootloader.o obj/protocol.o obj/consoleio.o