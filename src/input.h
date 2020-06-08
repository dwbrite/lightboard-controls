//
// Created by dwbrite on 5/22/20.
//

#ifndef LIGHTBOARD_INPUT_H
#define LIGHTBOARD_INPUT_H

#include <utility>
#include <vector>
#include <Arduino.h>
#include <unordered_map>

enum Direction { Left='<', Right='>' };
enum ActionType { Rotate, Depress, Release };


enum Actions {
    CtrlClick,
    CtrlLeft,
    CtrlRight,

    AltClick,
    AltLeft,
    AltRight,

    OptClick,
    OptLeft,
    OptRight
};

struct InterruptAxisInput {
    volatile float positiveState;
    volatile float negativeState;
};

struct PolledInput {
    float state;
    virtual void poll() = 0;
};

// input definitions
struct RotaryInput : InterruptAxisInput {
private:
    int lastEncoding;

public:
    int pinA; int pinB;

    RotaryInput(int pinA, int pinB);
    volatile void handleInput();
};

struct ButtonInput : PolledInput {
    int pin;

    ButtonInput(int pin);
    void poll();
};

enum AxisPolarity { Positive, Negative };

struct HalfAxisInput : PolledInput {
    AxisPolarity polarity;
    InterruptAxisInput *parent;

    HalfAxisInput(InterruptAxisInput *parent, AxisPolarity polarity);
    void poll();
};

// this is a bit incestuous now, but heck it, I'm tired of dealing with the C compiler >:(
using InputMap = std::unordered_map<Actions, std::vector<PolledInput*>>;

struct InputMapper {
    InputMap controls = InputMap();

    void mapControl(Actions action, PolledInput *input);
    void poll();
    volatile float getValue(Actions action);
};




#endif //LIGHTBOARD_INPUT_H
