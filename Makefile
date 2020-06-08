# set your MCU type here, or make command line `make MCU=MK20DX256`
MCU=MK20DX256

# make it lower case
LOWER_MCU := $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$(MCU)))))))))))))))))))))))))))
MCU_LD = $(LOWER_MCU).ld

# The name of your project (used to name the compiled .hex file)
TARGET = build/main

# configurable options
OPTIONS = -DF_CPU=144000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE

# options needed by many Arduino libraries to configure for Teensy 3.x
OPTIONS += -D__$(MCU)__ -DARDUINO=10805 -DTEENSYDUINO=144

CPUARCH = cortex-m4

COMPILERPATH ?= /usr/bin

#************************************************************************
# Settings below this point usually do not need to be edited
#************************************************************************

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -g -O3 -mcpu=$(CPUARCH) -mthumb -MMD $(OPTIONS)
CPPFLAGS += -Isrc -Isrc/modes -Isrc/util -Isrc/graphing -Ilib/cores/teensy3 -Ilib/SPI -Ilib/Adafruit_SSD1351 -Ilib/Adafruit_GFX -Ilib/FastLED
CPPFLAGS += -Wl,-u,vfprintf -lprintf_flt -lm

# compiler options for C++ only
CXXFLAGS = -std=gnu++14 -felide-constructors -fno-exceptions -fno-rtti


# linker options
LDFLAGS = -Wl,--gc-sections,--defsym=__rtc_localtime=0 --specs=nano.specs -mcpu=$(CPUARCH) -mthumb -T$(MCU_LD)

# additional libraries to link


# names for the compiler programs
CC = $(COMPILERPATH)/arm-none-eabi-gcc
CXX = $(COMPILERPATH)/arm-none-eabi-g++
OBJCOPY = $(COMPILERPATH)/arm-none-eabi-objcopy
SIZE = $(COMPILERPATH)/arm-none-eabi-size

# automatically create lists of the sources and objects
# TODO: this does not handle Arduino libraries yet...
C_FILES := $(wildcard lib/cores/teensy3/*.c) $(wildcard lib/SPI/*.c) $(wildcard lib/Adafruit_SSD1351/*.c) $(wildcard lib/Adafruit_GFX/*.c) $(wildcard lib/FastLED/*.c)
CPP_FILES := $(wildcard src/*.cpp) $(wildcard src/modes/*.cpp) $(wildcard src/util/*.cpp) $(wildcard src/graphing/*cpp) $(wildcard lib/cores/teensy3/*.cpp) $(wildcard lib/SPI/*.cpp) $(wildcard lib/Adafruit_SSD1351/*.cpp) $(wildcard lib/Adafruit_GFX/*.cpp) $(wildcard lib/FastLED/*.cpp)

OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all: $(TARGET).hex

clean:
	-rm src/*.o src/*.d
	-rm src/*/*.o src/*/*.d
	-rm lib/*/*.o lib/*/*.d
	-rm build/*

flash:
	teensy_loader_cli -s -mmcu=mk20dx256 $(TARGET).hex

$(TARGET).elf: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ -lm

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom $< $@
ifneq (,$(wildcard $(TOOLSPATH)))
	$(TOOLSPATH)/teensy_post_compile -file=$(basename $@) -path=$(shell pwd) -tools=$(TOOLSPATH)
	-$(TOOLSPATH)/teensy_reboot
endif

# include $(OBJS:.o=.d)

