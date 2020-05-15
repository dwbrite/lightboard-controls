#!/usr/bin/env bash

git clone git@github.com:PaulStoffregen/cores.git
rm cores/teensy3/main.cpp
git clone git@github.com:PaulStoffregen/SPI.git
git clone git@github.com:FastLED/FastLED.git
git clone git@github.com:adafruit/Adafruit-GFX-Library.git
mv "Adafruit-GFX-Library" "Adafruit_GFX"
git clone git@github.com:adafruit/Adafruit-SSD1351-Library.git
mv "Adafruit-SSD1351-Library" "Adafruit_SSD1351"