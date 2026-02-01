CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# 編譯參數：加入 nano.specs 和 syscalls.c 支援 printf
CFLAGS = -mcpu=cortex-m4 -mthumb -g -O0 --specs=nano.specs -nostartfiles

all: firmware.bin

# 這一行就是你報錯說找不到的規則，一定要有！
firmware.elf: main.c rcc.c uart.c syscalls.c startup.s
	$(CC) $(CFLAGS) -T linker.ld -o firmware.elf main.c rcc.c uart.c syscalls.c startup.s

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary firmware.elf firmware.bin

flash: firmware.bin
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program firmware.bin verify reset exit 0x08000000"

clean:
	rm -f *.o *.elf *.bin