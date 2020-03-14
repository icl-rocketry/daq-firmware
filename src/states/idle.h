/*
State that the DAQ boots up into, where it is waiting for instruction from
the web interface

Written by Electronics Team, Imperial College London Rocketry
*/
#include "Arduino.h"
#include "state.h"
#include "stateMachine.h"

#ifndef IDLE_H
#define IDLE_H

class Idle: public State {
  public:
    // Initialise the class by passing the correct ID to the state class
    Idle(): State(IDLE_STATE) {};

    void initialise();
    State* update();
};

#endif
