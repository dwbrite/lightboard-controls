#include <Arduino.h>
#include <core_pins.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <FastLED.h>
#include <queue>
#include "modes/modes.h"
#include "input.h"
#include "controller.h"

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

int main() {
    setup();

    while(true) {
        loop();
        yield();
    }
}

ControlContext controls = ControlContext();

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

    controls.altFns.singleClick = []{ mode[modeIdx]->alt.singleClick(); };
    controls.altFns.doubleClick = []{ mode[modeIdx]->alt.doubleClick(); };
    controls.altFns.tripleClick = []{ mode[modeIdx]->alt.tripleClick(); };
    controls.altFns.longClick = []{ mode[modeIdx]->alt.longClick(); };
    controls.altFns.turnLeft = []{ mode[modeIdx]->alt.turnLeft(); };
    controls.altFns.turnRight = []{ mode[modeIdx]->alt.turnRight(); };
    controls.altFns.turnLeftHeld = []{ mode[modeIdx]->alt.turnLeftHeld(); };
    controls.altFns.turnRightHeld = []{ mode[modeIdx]->alt.turnRightHeld(); };

    mode.push_back(new Sleep());
    mode.push_back(new SpinningRainbow(0, 32, 128, 48, 0));

    mode[modeIdx]->setup(tft);

    NVIC_SET_PRIORITY(IRQ_PORTA, 0);
    NVIC_SET_PRIORITY(IRQ_PORTB, 0);
    NVIC_SET_PRIORITY(IRQ_PORTC, 0);
    NVIC_SET_PRIORITY(IRQ_PORTD, 0);
    NVIC_SET_PRIORITY(IRQ_PORTE, 0);
}

void loop() {
    double deltaTime = (1000.0 / msSinceLoop);
    msSinceLoop = 0;

    //TODO: inputs
    controls.poll();

    mode[modeIdx]->clear(tft);
    mode[modeIdx]->update(deltaTime);
    mode[modeIdx]->drawScreen(tft);
    mode[modeIdx]->drawStrand(leds);


    if(true) {
        String s = "";
        if(Serial.available() > 0) {
            s = Serial.readString();
        }

        if(s.length()==2) {
            Serial.print("input: ");
            Serial.println(s);
            char knob = s.charAt(0);
            char direction = s.charAt(1);
            // TODO: send inputs to queue
        }

        mode[modeIdx]->showDebug(tft, deltaTime);
    }


#ifdef DEBUG
    String s = "";
    if(Serial.available() > 0) {
        s = Serial.readString();
    }

    if(s.length()==2) {
        Serial.print("input: ");
        Serial.println(s);
        char knob = s.charAt(0);
        char direction = s.charAt(1);
        // TODO: send inputs to queue
    }

    mode[modeIdx]->showDebug(tft, deltaTime);
#endif
}