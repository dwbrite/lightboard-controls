cmake_minimum_required(VERSION 3.16)
project(lightboard CXX)

set(MCU "MK20DX256")
set(CPUARCH "cortex-m4")
set(MCU_LD "mk20dx256.ld")

file(GLOB SOURCES src/*.cpp src/modes/*.cpp src/graphing/*.cpp)
file(GLOB LIB_TEENSY lib/cores/teensy3/*.c lib/cores/teensy3/*.cpp lib/cores/teensy3/avr/*.c lib/cores/teensy3/avr/*.cpp lib/cores/teensy3/util/*.c lib/cores/teensy3/util/*.cpp)
file(GLOB LIB_SPI lib/SPI/*.c lib/SPI/*.cpp)
file(GLOB LIB_GFX lib/Adafruit_GFX/*.c lib/Adafruit_GFX/*.cpp)
file(GLOB LIB_SSD1351 lib/Adafruit_SSD1351/*.cpp lib/Adafruit_SSD1351/*.c)
file(GLOB LIB_NEOPIXEL lib/FastLED/*.cpp lib/FastLED/*.c)

# configurable options
set(OPTIONS "-DF_CPU=48000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE")
set(OPTIONS "${OPTIONS} -D__${MCU}__ -DARDUINO=10805 -DTEENSYDUINO=144")

# compilation
set(LDFLAGS "-Os -Wl,--gc-sections,--defsym=__rtc_localtime=0 --specs=nosys.specs -mcpu=${CPUARCH} -mthumb")
set(CPPFLAGS "-Wall -g -Os -mcpu=${CPUARCH} -mthumb -MMD ${OPTIONS}")
# include dirs?
set(CPPFLAGS " ${CPPFLAGS} -Isrc -Isrc/modes -Isrc/util -Isrc/graphing -Ilib/cores/teensy3 -Ilib/SPI -Ilib/Adafruit_SSD1351 -Ilib/Adafruit_GFX -Ilib/FastLED")

include_directories(src src/modes src/util src/graphing lib/cores/teensy3 lib/SPI lib/Adafruit_SSD1351 lib/Adafruit_GFX lib/FastLED)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/arm-none-eabi-g++")
set(CMAKE_CXX_FLAGS "${CPPFLAGS} -std=gnu++14 -felide-constructors -fno-exceptions -fno-rtti")

add_executable(lightboard.elf ${SOURCES} ${LIB_TEENSY} ${LIB_SPI} ${LIB_GFX} ${LIB_SSD1351} ${LIB_NEOPIXEL} ${MCU_LD})
SET_TARGET_PROPERTIES(lightboard.elf PROPERTIES LINK_FLAGS ${LDFLAGS})
