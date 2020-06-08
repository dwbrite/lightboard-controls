//
// Created by dwbrite on 6/1/20.
//

#ifndef LIGHTBOARD_CONTROLLER_H
#define LIGHTBOARD_CONTROLLER_H

#include <elapsedMillis.h>
#include "input.h"


enum BtnEventType { SHORT_PRESS, LONG_PRESS };

struct BtnState {
    bool oldPress = false;
    bool heldTurn = false;
    elapsedMillis timeSincePress;
    elapsedMillis timeSinceRelease;
    unsigned long timeDepressed = 0;

    float threshold = 0.8;

    //time after release until the event is published
    unsigned long timeBuffer = 50;

    unsigned long longPressTime = 500;

    std::queue<BtnEventType> eventQueue;

    std::queue<BtnEventType> update(float value);

    bool isTurnHeld(float value);

    bool isDepressed(float value) { return value >= threshold; }

    void setThreshold(float value) { threshold = value; }

    void setTimeBuffer(unsigned long ms) { timeBuffer = ms; }
};

struct ControlCallbacks {
    void (*singleClick)() = []{};
    void (*doubleClick)() = []{};
    void (*tripleClick)() = []{};
    void (*longClick)() = []{};

    void (*turnLeft)() = []{};
    void (*turnRight)() = []{};
    void (*turnLeftHeld)() = []{};
    void (*turnRightHeld)() = []{};

    void executeBtnQueue(std::queue<BtnEventType> buttonEventQueue);
};

struct ControlContext {
    InputMapper map = InputMapper();

    BtnState altBtn;
    ControlCallbacks altFns;

    ControlContext();
    void poll();
};

#endif //LIGHTBOARD_CONTROLLER_H
