//
// Created by dwbrite on 5/7/20.
//

#ifndef LIGHTBOARD_MODES_H
#define LIGHTBOARD_MODES_H

#include <Arduino.h>
#include <core_pins.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <cmath>
#include "graphing/graphs.h"
#include <colors.h>
#include <FastLED.h>

struct ModeInputInterface {
    void (*singleClick)() = []{};
    void (*doubleClick)() = []{};
    void (*tripleClick)() = []{};
    void (*longClick)() = []{};
    void (*turnLeft)() = []{};
    void (*turnRight)() = []{};
    void (*turnLeftHeld)() = []{};
    void (*turnRightHeld)() = []{};
};

class IMode {
public:
    ModeInputInterface opt;
    ModeInputInterface alt;

    virtual void setup(Adafruit_SSD1351 tft) = 0;
    virtual void clear(Adafruit_SSD1351 tft) = 0;
    virtual void update(double dt) = 0;
    virtual void drawScreen(Adafruit_SSD1351 tft) = 0;
    virtual void drawStrand(CRGB *leds) = 0;

    virtual void showDebug(Adafruit_SSD1351 tft, double dt) {
        tft.fillRect(0, 128-12, 64, 12, BLACK);
        tft.setCursor(4, 128-12);
        tft.setTextColor(RED);
        tft.printf("%dfps", int(dt));
    }
};

class Sleep : public IMode {
    // no explicit constructor required
    void setup(Adafruit_SSD1351 tft);
    void clear(Adafruit_SSD1351 tft);
    void update(double dt);
    void drawScreen(Adafruit_SSD1351 tft);
    void drawStrand(CRGB *leds);
};

class SpinningRainbow : public IMode {
public:
    SpinningRainbow(int x, int y, int w, int h, uint16_t bg);

    void setup(Adafruit_SSD1351 tft);
    void clear(Adafruit_SSD1351 tft);
    void update(double dt);
    void drawScreen(Adafruit_SSD1351 tft);
    void drawStrand(CRGB *leds);

private:
    String mode_title = "rainbow";
    std::vector<Graph *> graphs;
    size_t graphIdx;

    uint16_t color565At(Graph *g, Adafruit_SSD1351 tft, float x);
    uint32_t colorAt(Graph *g, float x);
    void drawRainbowLine(Adafruit_SSD1351 tft);
    void showDebug(Adafruit_SSD1351 tft, double dt);
};

#endif //LIGHTBOARD_MODES_H