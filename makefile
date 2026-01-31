CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# 編譯選項 (針對 Cortex-M4)
CFLAGS = -mcpu=cortex-m4 -mthumb -g -O0 -nostdlib

all: firmware.bin

firmware.elf: main.c rcc.c uart.c startup.s
	$(CC) $(CFLAGS) -T linker.ld -o firmware.elf main.c uart.c rcc.c startup.s

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary firmware.elf firmware.bin

flash: firmware.bin
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program firmware.bin verify reset exit 0x08000000"

clean:
	rm -f *.o *.elf *.bin