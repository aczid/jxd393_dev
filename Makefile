CC = sdcc
AS = sdasstm8
LD = sdld

SRCDIR = src
OBJDIR = obj

C_FILES  = $(shell find $(SRCDIR) -name "*.c")
CFLAGS   = -mstm8
LDFLAGS  = -mstm8 -lstm8 
OBJECTS  = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.rel, $(C_FILES))
FIRMWARE = jxd393_stm8s005k6.ihx

$(OBJDIR)/%.rel: $(SRCDIR)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

.PHONY: all clean flash

all: $(FIRMWARE)

clean:
	rm -rf $(OBJDIR) $(FIRMWARE)

PROGRAMMER = stlinkv2
MCU = stm8s005k6
OPTION_BYTES_START = 0x4800
OPTION_BYTES_SIZE = 11
FLASH_START = 0x8000
FLASHER = stm8flash -c $(PROGRAMMER) -p $(MCU)

flash: $(FIRMWARE)
	$(FLASHER) -w $(FIRMWARE)

read_option_bytes:
	$(FLASHER) -s opt -b $(OPTION_BYTES_SIZE) -r options.bin
	@python print_options.py options.bin
	@rm options.bin

$(EEPROM): $(FIRMWARE)
	#objcopy -I ihex -O binary jxd393_stm8s005k6.ihx jxd393_stm8s005k6.bin

$(FIRMWARE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(FIRMWARE) $(LDFLAGS)

