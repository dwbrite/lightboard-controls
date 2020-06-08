//
// Created by dwbrite on 5/13/20.
//

#include <modes.h>

SpinningRainbow::SpinningRainbow(int x, int y, int w, int h, uint16_t bg) {
    // create a number of graphs and #fuckitup
    auto psin = new Graph(x, y, w, h, bg);
    auto arc = new Graph(x, y, w, h, bg);
    auto hsv = new Graph(x, y, w, h, bg);


    // sin^(2-sin) rainbow graph
    auto *r1 = new PSin(0b1111100000000000);
    auto *g1 = new PSin(0b0000011111100000);
    auto *b1 = new PSin(0b0000000000011111);
    g1->xOffset = 0.6667;
    b1->xOffset = 0.3333;

    psin->addFunction(r1);
    psin->addFunction(g1);
    psin->addFunction(b1);

    // arc-shaped rainbow graph
    auto *r2 = new RainbowArc(0b1111100000000000);
    auto *g2 = new RainbowArc(0b0000011111100000);
    auto *b2 = new RainbowArc(0b0000000000011111);
    r2->xOffset = +1.0/6;
    g2->xOffset = 0.6667+1.0/6;
    b2->xOffset = 0.3333+1.0/6;

    arc->addFunction(r2);
    arc->addFunction(g2);
    arc->addFunction(b2);

    // HSV based rainbow graph
    auto *r3 = new RainbowHSV(0b1111100000000000);
    auto *g3 = new RainbowHSV(0b0000011111100000);
    auto *b3 = new RainbowHSV(0b0000000000011111);
    r3->xOffset = -1.0/6;
    g3->xOffset = 0.6667-1.0/6;
    b3->xOffset = 0.3333-1.0/6;

    hsv->addFunction(r3);
    hsv->addFunction(g3);
    hsv->addFunction(b3);


    // add graphs and set the default
    graphs.push_back(arc);
    graphs.push_back(hsv);
    graphs.push_back(psin);

    graphIdx = 1;

    // set up interface
//    static auto turnLeft = [&]{
//        for (auto f : this->graphs[this->graphIdx]->functions) {
//            f->speed+=0.005;
//        }
//    };
    static auto self = this;
    alt.turnLeft = []{
        for (auto f : self->graphs[self->graphIdx]->functions) {
            f->speed+=0.005;
        }
    };

    alt.turnLeftHeld = []{
        for (auto f : self->graphs[self->graphIdx]->functions) {
            f->speed+=0.005;
        }
    };


    alt.turnRight = []{
        for (auto f : self->graphs[self->graphIdx]->functions) {
            f->speed-=0.005;
        }
    };

    alt.turnRightHeld = []{
        for (auto f : self->graphs[self->graphIdx]->functions) {
            f->speed-=0.005;
        }
    };


    alt.singleClick = []{
        for (auto f : self->graphs[self->graphIdx]->functions) {
            f->speed=0.0;
        }
    };
}

//void SpinningRainbow::optLeft() {};
//void SpinningRainbow::optClick() {};
//void SpinningRainbow::optRight() {};
//void SpinningRainbow::altLeft() {
//};
//void SpinningRainbow::altClick() {
//    for (auto f : graphs[graphIdx]->functions) {
//        f->speed=1;
//    }
//};
//void SpinningRainbow::altRight() {
//    for (auto f : graphs[graphIdx]->functions) {
//        f->speed-=0.005;
//    }
//};

void SpinningRainbow::setup(Adafruit_SSD1351 tft) {
    uint16_t bg = 0x0;

    tft.fillScreen(bg);
    tft.setCursor(0,0);
    tft.println(mode_title);
    Serial.println("mode: rainbowcycle");
}

void SpinningRainbow::update(double dt) {
    graphs[graphIdx]->update(dt);
}

void SpinningRainbow::drawRainbowLine(Adafruit_SSD1351 tft) {
    auto g = graphs[graphIdx];
    int x;
    int y;
    int w;
    uint16_t color;

    x = g->x0;
    y = g->y0 + g->getHeight() + 8;
    w = g->getWidth();
    for(int i = 0; i < w; i++) {
        color = color565At(g, tft, float(i) / g->getWidth());
        tft.drawFastVLine(i+x, y, 4, color);
        tft.drawPixel(i+x, y, color);
    }
//    color = cn(g, tft, 0);
//    tft.fillRect(x, y, 3, 3, color);
}

void SpinningRainbow::drawScreen(Adafruit_SSD1351 tft) {
    graphs[graphIdx]->draw(tft);
    drawRainbowLine(tft);
}

void SpinningRainbow::clear(Adafruit_SSD1351 tft) {
    // nothing to do, broadly speaking
}

void SpinningRainbow::drawStrand(CRGB *leds) {
    FastLED.clear(false);
    int size = FastLED.size();
    for(int x = 0; x < size; x++) {
        float xf = float(x)/size;
        uint32_t c = colorAt(graphs[graphIdx], xf);
        leds[x].setColorCode(c);
        // TODO: remove for when done testing
        leds[x].maximizeBrightness(32);
    }
    FastLED.show();
}

uint16_t SpinningRainbow::color565At(Graph *g, Adafruit_SSD1351 tft, float x) {
    int n = x*g->getWidth();
    uint8_t rgb[3];

    for(int c = 0; c < 3; c++) {
        Line l = g->cache[c][n];
        float val = 1 - (float(l.y1) / g->getHeight());
        rgb[c] = val * 255;
    }
    uint16_t color = tft.color565(rgb[0], rgb[1], rgb[2]);
    return color;
}


uint32_t SpinningRainbow::colorAt(Graph *g, float x) {
    int n = x*g->getWidth();
    uint8_t rgb[3];

    for(int c = 0; c < 3; c++) {
        Line l = g->cache[c][n];
        float val = 1 - (float(l.y1) / g->getHeight());
        rgb[c] = val * 255;
    }
    uint32_t color = (rgb[0]<<16) +  (rgb[1]<<8) + (rgb[2]);
    return color;
}

void SpinningRainbow::showDebug(Adafruit_SSD1351 tft, double dt) {
    IMode::showDebug(tft, dt);
    tft.fillRect(48, 118, 80, 8, 0);
    tft.setCursor(48, 118);
    auto f = graphs[graphIdx]->functions[0];
    char spd[16];
    dtostrf( f->speed, 2, 3, spd);
    tft.printf(spd);
}
