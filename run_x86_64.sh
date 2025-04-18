#!/bin/sh
unset GTK_PATH

#build 
pmn b x86_64-bootloader
make all

#make efi file
x86_64-elf-objcopy -I elf64-x86-64 -O efi-app-x86_64 bootLoader.elf BOOTX64.EFI

#make disk image
dd if=/dev/zero of=fat.img bs=1k count=1440
mformat -i fat.img -f 1440 ::
mmd -i fat.img ::/EFI
mmd -i fat.img ::/EFI/BOOT
mcopy -i fat.img BOOT*.EFI ::/EFI/BOOT
mmd -i fat.img ::/kernel
mcopy -i fat.img kernel.efi ::/kernel

#excute
# qemu-system-x86_64 -machine q35 -m 256 -smp 2 -bios OVMF.fd -drive file=fat.img,format=raw,if=virtio
qemu-system-x86_64 -machine q35 -m 256 -smp 2 -bios OVMF.fd -drive file=fat.img,format=raw,if=virtio -d cpu_reset -D qemu.log