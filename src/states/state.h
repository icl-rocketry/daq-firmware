/*
Parent class of the states

Written by Electronics Team, Imperial College London Rocketry
*/

#include "Arduino.h"

#ifndef STATE_H
#define STATE_H

const uint8_t IDLE_STATE = 1;
const uint8_t CALIBRATION_STATE = 2;
const uint8_t LOGGING_STATE = 3;

class State {
  public:

    State(uint8_t _stateID): stateID(_stateID) {};

    virtual void initialise();
    virtual State* update();

    uint8_t getID();

  protected:
    const uint8_t stateID;
};

#endif
