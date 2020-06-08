//
// Created by dwbrite on 6/2/20.
//

#include "input.h"

RotaryInput::RotaryInput(int pinA, int pinB) {
    this->pinA = pinA;
    this->pinB = pinB;

    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);

    digitalWrite(pinA, HIGH); // this doesn't make any sense to me
    digitalWrite(pinB, HIGH); //

    //static auto lambda = [&]{this->handleInput();};
    static auto self = this;
    ::attachInterrupt(pinA, []{ self->handleInput(); }, CHANGE);
    ::attachInterrupt(pinB, []{ self->handleInput(); }, CHANGE);
}

volatile void RotaryInput::handleInput()  {
    int signalA = digitalRead(pinA);
    int signalB = digitalRead(pinB);

    int encoding = (signalB << 1) | signalA;  // converting the 2 pin value to single number

    int sum  = (lastEncoding << 2) | encoding; // adding it to the previous encoded value

    // "simple" version
    if(sum == 0b0111) {
        positiveState = 1;
        negativeState = 0;
        Serial.printf("right: %d, %d\n",  int(negativeState), int(positiveState));
    } else if(sum == 0b0100) {
        positiveState = 0;
        negativeState = 1;
        Serial.printf("left: %d, %d\n",  int(negativeState), int(positiveState));
    }

    // normal version
//        if(sum == 0b0111 || sum == 0b1110 || sum == 0b1000 || sum == 0b0001) {
//            if (negativeState > 0) {
//                positiveState = 1;
//            } else {
//                positiveState = 0;
//            }
//            negativeState = 0;
//            Serial.println("LEFT>");
//        } else if(sum == 0b1101|| sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
//            if (positiveState > 0) {
//                negativeState = 1;
//            } else {
//                negativeState = 0;
//            }
//            positiveState = 0;
//            Serial.println("RIGHT>");
//        }

    lastEncoding = encoding;
}

HalfAxisInput::HalfAxisInput(InterruptAxisInput *parent, AxisPolarity polarity)  {
    this->parent = parent;
    this->polarity = polarity;
}

void HalfAxisInput::poll()  {
    switch (polarity) {
        case Positive:
            state = parent->positiveState;
            parent->positiveState = 0;
            break;
        case Negative:
            state = parent->negativeState;
            parent->negativeState = 0;
            break;
    }
}

void InputMapper::mapControl(Actions action, PolledInput *input) {
    if (controls.find(action) == controls.end())
        controls.insert(std::make_pair(action, std::vector<PolledInput*>()));

    controls[action].push_back(input);
}

volatile float InputMapper::getValue(Actions a) {
    float result = 0.0;
    for(auto & in : controls[a]) {
        result = std::max(in->state, result);
    }

    if(result > 1.0) {
        result = 1.0;
    }

    return result;
}

void InputMapper::poll() {
    for(auto e : controls) {
        for(auto & in : e.second) {
            in->poll();
        }
    }
}

void ButtonInput::poll() { state = !digitalRead(pin); }

ButtonInput::ButtonInput(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP); // I think?
}