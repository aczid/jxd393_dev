CC = sdcc
AS = sdasstm8
LD = sdld

SRCDIR     = src
OBJDIR     = obj
STM8LIBDIR = $(SRCDIR)/stm8s_stdlib

C_FILES    = $(shell find $(SRCDIR) -name "*.c" \! -path "$(STM8LIBDIR)*")
ST_C_FILES = $(shell find $(STM8LIBDIR) -name "*.c")
CFLAGS     = -mstm8 -Wa,-l -I $(SRCDIR) -I $(STM8LIBDIR)
LDFLAGS    = -mstm8 -lstm8 
OBJECTS    = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.rel, $(C_FILES))
ST_OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.rel, $(ST_C_FILES))

PROGRAMMER = stlinkv2
MCU        = stm8s005k6
FLASHER    = stm8flash -c $(PROGRAMMER) -p $(MCU)
BUILD      = jxd393_$(MCU)
FIRMWARE   = $(BUILD).ihx

$(OBJDIR)/%.rel: $(SRCDIR)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

.PHONY: all clean flash

all: $(FIRMWARE)

clean:
	rm -rf $(OBJECTS) $(FIRMWARE) $(BUILD).map $(BUILD).lk

distclean: clean
	rm -rf $(OBJDIR)

$(FIRMWARE): $(OBJECTS) $(ST_OBJECTS)
	$(CC) $(ST_OBJECTS) $(OBJECTS) -o $(FIRMWARE) $(LDFLAGS)

flash: $(FIRMWARE)
	$(FLASHER) -w $(FIRMWARE)

## Debugging / configure options from here

OPTION_BYTES_SIZE = 11
OPTIONS_BIN       = opt.bin
RAM_BIN           = ram.bin

reset_options:
	echo "Resetting options"
	perl -e 'print "\x00" . "\x00\xff"x6 . "\xff\x00"' > $(OPTIONS_BIN)
	$(FLASHER) -s opt -w $(OPTIONS_BIN)
	@rm $(OPTIONS_BIN)

enable_rop:
	echo "Enabling ROP"
	perl -e 'print "\xaa" . "\x00\xff"x6 . "\xff\x00"' > $(OPTIONS_BIN)
	$(FLASHER) -s opt -w $(OPTIONS_BIN)
	@rm $(OPTIONS_BIN)

disable_rop:
	echo "Disabling ROP"
	perl -e 'print "\x00"' > $(OPTIONS_BIN)
	$(FLASHER) -s opt -w $(OPTIONS_BIN)
	@rm $(OPTIONS_BIN)

read_option_bytes:
	$(FLASHER) -s opt -b $(OPTION_BYTES_SIZE) -r $(OPTIONS_BIN)
	@python print_options.py $(OPTIONS_BIN)
	@rm $(OPTIONS_BIN)

print_debug_buffer:
	$(FLASHER) -s ram -r $(RAM_BIN)
	@xxd -l 64 -s +`grep -bao DBG $(RAM_BIN) | cut -d':' -f 1` -s+4 $(RAM_BIN)
	#@rm $(RAM_BIN)
