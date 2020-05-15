###requirements:
- arm-none-eabi-gcc
- teensy-loader-cli

`sudo apt install gcc-arm-none-eabi teensy-loader-cli`

####libs:
- Adafruit-GFX-Library
- Adafruit-SSD1351-Library
- cores/teensy3
- Teensy SPI
- FastLED

`cd libs; ./clone_libs.sh`

### build
- `make clean` to clean
- `make` to compile
- `make flash` to flash

CMakeLists.txt is exclusively for CLion code completion