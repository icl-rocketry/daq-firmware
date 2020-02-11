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

#ifndef LOGGING_H
#define LOGGING_H

class Logging: public State {
  public:
    void init();
    void update();
}

#endif
