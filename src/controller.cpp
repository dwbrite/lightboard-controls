//
// Created by dwbrite on 6/1/20.
//

#include <queue>
#include "controller.h"
#include "input.h"

std::queue<BtnEventType> BtnState::update(float value) {
    std::queue<BtnEventType> result;

    bool depressed = isDepressed(value);
    if(depressed && !oldPress) {
        timeSincePress = 0;
        oldPress = true;
    } else if(!depressed && oldPress) {
        oldPress = false;
        timeSinceRelease = 0;
        timeDepressed = timeSincePress;

        if(heldTurn) {
            heldTurn = false;
            return result;
        }

        // push the click event to the queue
        if(timeDepressed >= longPressTime) {
            eventQueue.push(LONG_PRESS);
        } else {
            eventQueue.push(SHORT_PRESS);
        }
    } else if(!depressed && (timeSinceRelease > timeBuffer || eventQueue.size() >= 3)) {
        // is this the most effective way to do this?
        while (!eventQueue.empty()) {
            result.push(eventQueue.front());
            eventQueue.pop();
        }
    }
    return result;
}

bool BtnState::isTurnHeld(float value) {
    if(isDepressed(value)) {
        heldTurn = true;
    }
    return heldTurn;
}


void ControlCallbacks::executeBtnQueue(std::queue<BtnEventType> buttonEventQueue) {
    int clickCounter = 0;
    while(!buttonEventQueue.empty()) {
        if (buttonEventQueue.front() == SHORT_PRESS) {
            buttonEventQueue.pop();
            ++clickCounter;
        } else {
            buttonEventQueue.pop();
            longClick();
        }

    }
    switch(clickCounter) {
        case 1: singleClick(); break;
        case 2: doubleClick(); break;
        case 3: tripleClick(); break;
        default: break;
    }
}

ControlContext::ControlContext() {
    auto *spAltRot = new RotaryInput(4, 5);

    map.mapControl(AltLeft, new HalfAxisInput(spAltRot, Negative));
    map.mapControl(AltRight, new HalfAxisInput(spAltRot, Positive));
    map.mapControl(AltClick, new ButtonInput(6));
}

void ControlContext::poll() {
    map.poll();

    auto altValue = map.getValue(AltClick);
    auto altBtnQueue = altBtn.update(altValue);
    //auto optBtnQueue =

    for (auto e : map.controls) {
        switch (e.first) {
            case AltClick:
                altFns.executeBtnQueue(altBtnQueue);
                break;
            case AltLeft:
                if(map.getValue(AltLeft) == 1.0) {
                    if(altBtn.isTurnHeld(altValue)) {
                        altFns.turnLeftHeld();
                    } else {
                        altFns.turnLeft();
                    }
                }
                break;
            case AltRight:
                if(map.getValue(AltRight) == 1.0) {
                    if(altBtn.isTurnHeld(altValue)) {
                        altFns.turnRightHeld();
                    } else {
                        altFns.turnRight();
                    }
                }
            default:
                break;
        }
    }
}