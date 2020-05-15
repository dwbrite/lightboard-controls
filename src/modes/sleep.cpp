//
// Created by dwbrite on 5/13/20.
//

#include <modes.h>

void Sleep::altLeft() {}
void Sleep::altClick() {}
void Sleep::altRight() {}
void Sleep::optLeft() {}
void Sleep::optClick() {}
void Sleep::optRight() {}

void Sleep::setup(Adafruit_SSD1351 tft) {
    tft.fillScreen(BLACK);
    tft.setTextColor(RED);
    tft.setCursor(0,0);
    tft.println("zzz");
}

void Sleep::update(double dt) {}

void Sleep::drawScreen(Adafruit_SSD1351 tft) {}

void Sleep::clear(Adafruit_SSD1351 tft) {}

void Sleep::drawStrand(CRGB *leds) {
    FastLED.clear(true);
    FastLED.show();
}