

all: libbob3.a

##################
# Compiler setup
PREFIX = /opt/avr/avr-gcc-4.3.4/bin/
GCC_VERSION=-4.3.4

# PREFIX =
# GCC_VERSION=

##################
# AVR compiler setup
CC = $(PREFIX)avr-gcc$(GCC_VERSION)
OBJCOPY = $(PREFIX)avr-objcopy
OBJDUMP = $(PREFIX)avr-objdump
SIZE = $(PREFIX)avr-size
AR = $(PREFIX)avr-ar

##################
# Platforms
LIBS =

##################
# Math and printf
MATH =
PRINTF =


##################
# Platform
DEVICE = atmega88
F_CPU = 8000000
PLATFORM = B-O-B-3
LIBFOLDER = m88-8
EFUSE=07
HFUSE=df
LFUSE=e2

##################
# NIBObee Lib
CFLAGS += -D_BOB3_
CCFLAGS += -D_BOB3_
PROGRAMMER = avrispv2
CFLAGS += -std=c99 -Os -ffunction-sections -DAVR -Wall -I..
CCFLAGS += -Os -ffunction-sections -DAVR -Wall -I..


##################
# Standard Flags
CFLAGS += -mmcu=$(DEVICE) -DF_CPU=$(F_CPU)
CCFLAGS += -mmcu=$(DEVICE) -DF_CPU=$(F_CPU)
CLDFLAGS += -mmcu=$(DEVICE)


##################
# General Rules
LINECOUNT_CMD = 'a=`cat \$0|wc -l` ; echo "compiled $a lines" >>compiler.err'

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	@$(SIZE) -B $@ >>sizes.log

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@
	@$(SIZE) -B $@ >>sizes.log

libbob3.a: analog.o  bob3.o ircom.o leds.o main.o

%.a:
	rm -f $@
	$(AR) -q $@ $^

clean:
	rm -f *.d *.o *~ *.elf *.log *.err $(BASENAME).hex $(TEMP_FILES) $(BASENAME).xhex $(BASENAME).lss


.PHONY: all clean clean-errors %.c-compile link-success compile-success compile-info link-info build-success
.SECONDARY: $(BASENAME).hex $(BASENAME).xhex $(BASENAME).elf $(BASENAME).o $(OBJS) $(LOBJS)
