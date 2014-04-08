#=================================================================================================
#Beryllium Build System
#=================================================================================================
ARCH :=x86
ARCH_DIRECTORY := src/${ARCH}

COMPILE_OPTIONS := -D DEBUG -D ENABLE_SERIAL -D LOG_SERIAL -DARCH=${ARCH} -DARCH_STRING="\"${ARCH}\"" #-D KERNEL_SYMBOLS #-D KLOG_TITLE_TIME
#Files
BOOT_FILES := $(patsubst %.c,%.o,$(wildcard src/boot/*.c))
ARCH_BOOT_FILES := $(patsubst %.s,%.o,$(wildcard ${ARCH_DIRECTORY}/boot/*.s)) $(patsubst %.c,%.o,$(wildcard ${ARCH_DIRECTORY}/boot/*.c))
ARCH_LOW_FILES := $(patsubst %.c,%.o,$(wildcard ${ARCH_DIRECTORY}/low/*.c)) $(patsubst %.s,%.o,$(wildcard ${ARCH_DIRECTORY}/low/*.s))
LIB_FILES := $(patsubst %.c,%.o,$(wildcard src/lib/*.c))
ARCH_LIB_FILES := $(patsubst %.c,%.o,$(wildcard ${ARCH_DIRECTORY}/lib/*.c))
DRIVER_FILES := $(patsubst %.c,%.o,$(wildcard src/drivers/*.c))
ARCH_DRIVER_FILES  := $(patsubst %.c,%.o,$(wildcard ${ARCH_DIRECTORY}/drivers/*.c))
KERNEL_FILES := $(patsubst %.c,%.o,$(wildcard src/*.c))
ARCH_FILES := $(patsubst %.c,%.o,$(wildcard ${ARCH_DIRECTORY}/*.c))
FS_FILES := $(patsubst %.c,%.o,$(wildcard src/fs/*.c))
SRC_FILES := ${BOOT_FILES} ${KERNEL_FILES} ${DRIVER_FILES} ${LIB_FILES} ${ARCH_FILES} ${ARCH_BOOT_FILES} ${ARCH_LOW_FILES} ${ARCH_LIB_FILES} ${ARCH_DRIVER_FILES} ${FS_FILES}
#Compiler Options
CC:=clang -DX86 -target i586-elf
CPP:=clang++
C_OPTIONS := -ffreestanding -std=gnu99 -nostdlib -nostartfiles -fno-builtin -nostartfiles
C_OPTIONS += -Wall -Wextra -Wno-unused-function -Wno-unused-parameter
C_OPTIONS += -Wno-unused-function -Wno-unused-parameter 
LD := ./toolkit/binutils/bin/i586-elf-ld -m elf_i386
LFLAGS :=
LD_SCRIPT := ${ARCH_DIRECTORY}/link.ld
INCLUDE_DIR := "./src/includes"
CROSS_CLANG := -target i586-elf
ASM := nasm -f elf 
GENISO := xorriso -as mkisofs
#Rules
.PHONY: iso clean

all:kernel gen-symbols add-symbols iso

arch-boot: ${ARCH_BOOT_FILES}
boot: ${BOOT_FILES}

arch-low: ${ARCH_LOW_FILES}

lib: ${LIB_FILES}
arch-lib: ${ARCH_LIB_FILES}

arch-files: ${ARCH_FILES}

drivers: ${DRIVER_FILES}
arch-drivers: ${ARCH_DRIVER_FILES}

fs: ${FS_FILES}
kernel: arch-boot boot lib drivers arch-files arch-low arch-lib arch-drivers fs ${KERNEL_FILES}
	@echo "Linking Kernel"
	@echo ${LFLAGS}
	@${LD} ${LFLAGS} -T ${LD_SCRIPT} -o kernel.elf ${SRC_FILES}

#Generic

%.o: %.s
	@echo "Making: " $@
	@${ASM} -o $@ $<

%.o: %.c
	@echo "Making: " $@
	@${CC} -c ${C_OPTIONS} ${COMPILE_OPTIONS} -DGITHASH=${GIT_HASH} -I${INCLUDE_DIR} -o $@ $<

%.o: %.cpp
	@echo "Making: " $@
	@${CPP} -c ${CPP_OPTIONS}  ${COMPILE_OPTIONS} -I${INCLUDE_DIR} -o $@ $<

clean: prep-dist
	-rm -rf src/*.o src/boot/*.o src/lib/*.o src/drivers/*.o src/fs/*.o ${ARCH_DIRECTORY}/*.o ${ARCH_DIRECTORY}/boot/*.o ${ARCH_DIRECTORY}/drivers/*.o ${ARCH_DIRECTORY}/lib/*.o ${ARCH_DIRECTORY}/low/*.o
	-rm -rf util/*.o util/*.bin
	-rm -rf *.iso
	-rm -rf kernel.elf kernel.img

prep-dist:
	-rm -rf *~ boot/*~ src/*~

run:
	@echo "Remember! Use make run to test the kernel! Implement it into a OS if you wish to test other fuctions!"
	qemu-system-i386 -serial stdio -cdrom cdrom.iso

iso:
	@echo "Creating ISO..."
	@cp kernel.elf iso/kernel.elf
	@${GENISO} -R -b boot/grub/stage2_eltorito -quiet -no-emul-boot -boot-load-size 4 -boot-info-table -o cdrom.iso iso

util: util-iboot-iso
	@echo "Built Utilities"

util/iboot.bin:
	@nasm util/iboot-iso.s -f bin -o util/iboot.bin

util-iboot: util/iboot.bin
	@cp util/iboot.bin iso/boot/iboot.bin
	@echo "Integrated Bootloader Built"

util-iboot-iso: util-iboot
	@echo "Creating iboot ISO..."
	@cp kernel.elf iso/kernel.elf
	@${GENISO} -R -J -c boot/bootcat -b boot/iboot.bin -no-emul-boot -boot-info-table -boot-load-size 4 iso -o iboot.iso

gen-symbols:
	nm kernel.elf > kernel.map

add-symbols:
	cp kernel.map iso/boot/symbols.mod
x86:
	@make all iso
arm:
	@make integrator-cp
integrator-cp:
	@make kernel ARCH=arm/integrator-cp ASM=arm-none-eabi-as LD="arm-none-eabi-gcc -lgcc -nostartfiles -fno-builtin -nostartfiles" LFLAGS="" CC="arm-none-eabi-gcc -DARM"
run-arm:
	@qemu-system-arm -m 8 -serial stdio -kernel kernel.elf