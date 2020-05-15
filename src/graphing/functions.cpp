//
// Created by dwbrite on 5/13/20.
//

#include <math.h>
#include "functions.h"

PSin::PSin(uint16_t color) {
    this->color = color;
}

double PSin::f(double x) {
    float z = fmodf(x+xOffset, 1.0);
    float y;

    if(0.5 < z && z < 0.5+(2.0/6)) {
        y = -1;
    } else {
        if(0.5+(2.0/6) <= z && z < 1) {
            y = sinf(3 * z * pi - pi);
        } else {
            y = sinf(3 * z * pi);
        }
    }

    y/=2;
    y+=0.5;
    y*=2-y;
    y-=0.5;
    y*=2;

    return y;
}

void PSin::update(double dt) {
    calcXOffsetDelta(dt);
}

RainbowArc::RainbowArc(uint16_t color) {
    this->color = color;
}

double RainbowArc::f(double x) {
    double y = 0.0;
    uint8_t denom = 3;
    uint8_t e = 2;
    double p = 2.0/denom;

    double xo = x + xOffset;

    y = fmod(xo, 1);
    if (y > p) {
        y = 0;
    }
    y = abs(y - 1.0/denom);
    y = pow(y, e);
    y = pow(denom, e) * y;
    y = abs(y - 1);

    // convert from 0..1 to -1..1
    return 2*y-1;
}

void RainbowArc::update(double dt) {
    calcXOffsetDelta(dt);
}

RainbowHSV::RainbowHSV(uint16_t color) {
    this->color=color;
}

double RainbowHSV::f(double x) {
    double h = fmod(x+xOffset, 1);
    double v = 1;
    double s = 1;
    double c = v*s;
    double X = c*(1 - abs(fmodf(h/(1.0/6),2)-1));
    double m = v-c;

    double y = 0;

    if ((1.0/6 <= h && h < 2.0/6) || (4.0/6 <= h && h < 5.0/6)) {
        y = X;
    } else if (2.0/6 <= h && h < 4.0/6) {
        y = 0;
    } else {
        y = c;
    }

    // convert from 0..1 to -1..1
    return 2*y-1;
}

void RainbowHSV::update(double dt) {
    calcXOffsetDelta(dt);
}