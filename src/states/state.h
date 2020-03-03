/*
Parent class of the states

Written by Electronics Team, Imperial College London Rocketry
*/

#include "Arduino.h"

#ifndef STATE_H
#define STATE_H

class State {
  public:
    virtual void initialise();
    virtual State* update();
    int8_t getID();
    int8_t stateID;
};

#endif
