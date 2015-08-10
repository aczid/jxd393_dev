CC = sdcc
AS = sdasstm8
LD = sdld

SRCDIR = src
OBJDIR = obj

C_FILES  = $(shell find $(SRCDIR) -name "*.c")
CFLAGS   = -mstm8 -DSTM8S005 -Wa,-l -I src -I src/stm8_stdlib
LDFLAGS  = -mstm8 -lstm8 
OBJECTS  = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.rel, $(C_FILES))
FIRMWARE = jxd393_stm8s005k6.ihx

OPTIONS_BIN = opt.bin
PROGRAMMER = stlinkv2
MCU = stm8s005k6
OPTION_BYTES_START = 0x4800
OPTION_BYTES_SIZE = 11
FLASH_START = 0x8000
FLASHER = stm8flash -c $(PROGRAMMER) -p $(MCU)

$(OBJDIR)/%.rel: $(SRCDIR)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

.PHONY: all clean flash

all: $(FIRMWARE)

clean:
	rm -rf $(OBJDIR) $(FIRMWARE) $(OPTIONS_BIN)

reset_options:
	echo "Resetting options"
	perl -e 'print "\x00" . "\x00\xff"x6 . "\xff\x00"' > $(OPTIONS_BIN)
	stm8flash -c stlinkv2 -p stm8s005k6 -s opt -w $(OPTIONS_BIN) -b 15

enable_rop:
	echo "Enabling ROP"
	perl -e 'print "\xaa" . "\x00\xff"x6 . "\xff\x00"' > $(OPTIONS_BIN)
	stm8flash -c stlinkv2 -p stm8s005k6 -s opt -w $(OPTIONS_BIN) -b 1

disable_rop:
	echo "Disabling ROP"
	perl -e 'print "\x00"' > $(OPTIONS_BIN)
	stm8flash -c stlinkv2 -p stm8s005k6 -s opt -w $(OPTIONS_BIN) -b 1 -m

flash: $(FIRMWARE)
	$(FLASHER) -w $(FIRMWARE)

read_option_bytes:
	$(FLASHER) -s opt -b $(OPTION_BYTES_SIZE) -r $(OPTIONS_BIN)
	@python print_options.py $(OPTIONS_BIN)
	@rm $(OPTIONS_BIN)

print_debug_buffer:
	$(FLASHER) -s ram -r ram.bin
	xxd -s 0x`strings -a -t x ram.bin | grep DBG -A 1 | tail -n1 | cut -d' ' -f7` -l 128 ram.bin 
	@rm ram.bin

$(FIRMWARE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(FIRMWARE) $(LDFLAGS)

