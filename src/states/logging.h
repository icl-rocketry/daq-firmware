/*
State in which the DAQ is reading the data from the load cell, while the DAQ
is reading the data from the test fire. 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CAUTION: THE E-MATCH FIRES UPON ENTERING THIS STATE
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Written by Electronics Team, Imperial College London Rocketry
*/
#include "Arduino.h"
#include "state.h"
#include "stateMachine.h"

#ifndef LOGGING_H
#define LOGGING_H

class Logging: public State {
  public:
    // Initialise the class by passing the correct ID to the state class
    Logging(): State(LOGGING_STATE) {};

    void initialise();
    State* update();
};

#endif
