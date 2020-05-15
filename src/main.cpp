#include <Arduino.h>
#include <core_pins.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <FastLED.h>
#include "modes/modes.h"

// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

// You can use any (4 or) 5 pins
#define SCLK_PIN 13
#define MOSI_PIN 11
#define CS_PIN   10
#define DC_PIN   9
#define RST_PIN  8

#define PXL_PIN   2
#define NUMPIXELS 29

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);
CRGB leds[NUMPIXELS];


std::vector<IMode *> mode = std::vector<IMode *>();
int modeIdx = 1;
int lastActive = 1;

elapsedMillis msSinceLoop;

int main();
void setup();
void setupEncoder(int pinA, int pinB, int pinClick);

void executeInputs(char knob, char direction);

void modeInput(char direction);

void altInput(char direction);

void optInput(char direction);

enum Knob {Mode='0', Opt='1', Alt='2'};
enum Input {Left='<', Click='.', Right='>'};

int main() {
    setup();

    while(true) {
        loop();
        yield();
    }
}


// TODO: REMOVE
int pinAltA = 4;
int pinAltB = 5;
int pinAltClick = 6;
volatile int encAltLastEncode = 0b11;

void handleInput() {
    int altSignalA = digitalRead(pinAltA);
    int altSignalB = digitalRead(pinAltB);

    int altEncoded = (altSignalB << 1) | altSignalA;  // converting the 2 pin value to single number
    int altSum  = (encAltLastEncode << 2) | altEncoded; // adding it to the previous encoded value

    if(altSum == 0b0111 || altSum == 0b1110 || altSum == 0b1000 || altSum == 0b0001) {
        altInput('>');
    }
    if(altSum == 0b1101|| altSum == 0b0100 || altSum == 0b0010 || altSum == 0b1011) {
        altInput('<');
    }

    encAltLastEncode = altEncoded; // store this value for next time
}

void setup() {
    Serial.begin(9600);
    tft.begin();
    tft.fillScreen(0x0);
    tft.print("chillin'");
    delay(250);
    tft.print(".");
    delay(250);
    tft.print(".");
    delay(250);
    tft.println(".");
    Serial.println("init");
    delay(250);


    FastLED.addLeds<NEOPIXEL, PXL_PIN>(leds, NUMPIXELS);
    FastLED.clear(true);

    setupEncoder(pinAltA, pinAltB, pinAltClick);

    mode.push_back(new Sleep());
    mode.push_back(new SpinningRainbow(0, 32, 128, 48, 0));

    mode[modeIdx]->setup(tft);
}

void setupEncoder(int pinA, int pinB, int pinClick) {
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pinMode(pinClick, INPUT);

    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);

    attachInterrupt(pinA, handleInput, CHANGE);
    attachInterrupt(pinB, handleInput, CHANGE);
}

void loop() {
    double deltaTime = (1000.0 / msSinceLoop);
    msSinceLoop = 0;

    String s = "";
    if(Serial.available() > 0) {
        s = Serial.readString();
    }

    if(s.length()==2) {
        Serial.print("input: ");
        Serial.println(s);
        char knob = s.charAt(0);
        char direction = s.charAt(1);
        executeInputs(knob, direction);
    }

    mode[modeIdx]->clear(tft);
    mode[modeIdx]->update(deltaTime);
    mode[modeIdx]->drawScreen(tft);
    mode[modeIdx]->drawStrand(leds);

    auto debug = true;
    if (debug) {
        mode[modeIdx]->showDebug(tft, deltaTime);
    }
}

void executeInputs(char knob, char direction) {
    switch(knob) {
        case Mode: modeInput(direction); break;
        case Opt: optInput(direction); break;
        case Alt: altInput(direction); break;
        default: return;
    }
}

void optInput(char direction) {
    switch(direction) {
        case Left: mode[modeIdx]->optLeft(); break;
        case Click: mode[modeIdx]->optClick(); break;
        case Right: mode[modeIdx]->optRight(); break;
    }
}

void altInput(char direction) {
    switch(direction) {
        case Left: mode[modeIdx]->altLeft(); break;
        case Click: mode[modeIdx]->altClick(); break;
        case Right: mode[modeIdx]->altRight(); break;
    }
}

void modeInput(char direction) {
    // TODO: make this safe? in theory it should never fail as long as you have at least 2 modes...
    switch(direction) {
        case Left:
            if(--modeIdx <= 0) {
                modeIdx = mode.size()-1;
            }
            break;
        case Click:
            if(modeIdx == 0) {
                modeIdx = lastActive;
            } else {
                lastActive = modeIdx;
                modeIdx = 0;
            }
            break;
        case Right:
            if(++modeIdx >= int(mode.size())) {
                modeIdx = 1;
            }
            break;
        default: return;
    }
    mode[modeIdx]->setup(tft);
}