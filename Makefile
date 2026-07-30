# --------------------------------------------------------
# Toolchain
# --------------------------------------------------------

CC      := aarch64-none-elf-gcc
OBJCOPY := aarch64-none-elf-objcopy

# --------------------------------------------------------
# Directories
# --------------------------------------------------------

SRC_DIR := src
INC_DIR := include
BUILD   := build

# --------------------------------------------------------
# Sources
# --------------------------------------------------------

SRC_C := $(wildcard $(SRC_DIR)/*.c)
SRC_S := $(wildcard $(SRC_DIR)/*.S)

OBJ := \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRC_C)) \
	$(patsubst $(SRC_DIR)/%.S,$(BUILD)/%.o,$(SRC_S))

# --------------------------------------------------------
# Compiler Flags
# --------------------------------------------------------

CFLAGS := \
	-Wall \
	-O2 \
	-ffreestanding \
	-nostdlib \
	-nostartfiles \
	-I$(INC_DIR)

# --------------------------------------------------------
# Targets
# --------------------------------------------------------

all: kernel8.img

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(SRC_DIR)/%.S | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -T linker.ld -o $@

kernel8.img: $(BUILD)/kernel.elf
	$(OBJCOPY) $< -O binary $@

run: kernel8.img
	qemu-system-aarch64 \
		-M raspi3b \
		-cpu cortex-a53 \
		-kernel kernel8.img \
		-serial stdio \
		-display none

clean:
	rm -rf $(BUILD) kernel8.img

.PHONY: all run clean

