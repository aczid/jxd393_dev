CC = sdcc
LD = sdld

SRCDIR = src
OBJDIR = obj

C_FILES  = $(shell find $(SRCDIR) -name "*.c" \! -name "._*.c")
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

flash: $(FIRMWARE)
	stm8flash -c $(PROGRAMMER) -p $(MCU) -w $(FIRMWARE)

read_option_bytes:
	stm8flash -c $(PROGRAMMER) -p $(MCU) -s $(OPTION_BYTES_START) -b $(OPTION_BYTES_SIZE) -r options.bin
	@python print_options.py options.bin
	@rm options.bin

$(FIRMWARE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(FIRMWARE) $(LDFLAGS)

