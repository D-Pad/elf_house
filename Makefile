# Cross compiler tools
CC      = aarch64-none-elf-gcc
OBJCOPY = aarch64-none-elf-objcopy

# Compiler flags
CFLAGS = -Wall -O2 -ffreestanding -nostdlib -nostartfiles

# Linker flags
LDFLAGS = -T linker.ld

# Source files
SRC_C = main.c
SRC_S = boot.S

# Object files
OBJ = build/main.o build/boot.o

all: kernel8.img

build:
	mkdir -p build

build/main.o: main.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/boot.o: boot.S | build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

kernel8.img: build/kernel.elf
	$(OBJCOPY) $< -O binary $@

run: kernel8.img
	qemu-system-aarch64 \
		-M raspi3b \
		-cpu cortex-a53 \
		-kernel kernel8.img \
		-serial stdio \
		-display none

clean:
	rm -rf build kernel8.img

.PHONY: all run clean
