/*
Parent class of the states

Written by Electronics Team, Imperial College London Rocketry
*/

#include "Arduino.h"

#ifndef STATE_H
#define STATE_H

const uint8_t IDLE_STATE = 70;
const uint8_t CALIBRATION_STATE = 6;
const uint8_t LOGGING_STATE = 68;

class State {
  public:
    virtual void initialise();
    virtual State* update();
    virtual uint8_t getID();
  protected:
    uint8_t stateID;
};

#endif
