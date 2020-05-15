//
// Created by dwbrite on 5/13/20.
//

#ifndef LIGHTBOARD_FUNCTIONS_H
#define LIGHTBOARD_FUNCTIONS_H

#include <cstdint>

#define pi 3.1415926
#define tau (pi*2)

class GraphingFunction {
public:
    uint16_t color;
    double xOffset = 0;
    double yOffset = 0;
    volatile double speed = 0.2;

    virtual double f(double x) = 0;
    virtual void update(double dt) = 0;
    void calcXOffsetDelta(double dt) {
        xOffset+=speed/dt;
        if(xOffset < 0) {
            xOffset = 1+fmod(xOffset, -1);
        }
        if(xOffset > 1) {
            xOffset = fmod(xOffset, 1);
        }
    }
};

class PSin : public GraphingFunction {
public:
    PSin(uint16_t color = 0xFFFF);

    double f(double x);
    virtual void update(double dt);
};

class RainbowArc : public GraphingFunction {
public:
    RainbowArc(uint16_t color = 0xFFFF);

    double f(double x);
    virtual void update(double dt);
};

class RainbowHSV : public GraphingFunction {
public:
    RainbowHSV(uint16_t color = 0xFFFF);
    double f(double x);
    virtual void update(double dt);
};

#endif //LIGHTBOARD_FUNCTIONS_H
