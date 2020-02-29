/*
State in which the DAQ reads data from the load cell to a separate file,
such that it can be calibrated using a known mass. It awaits a change of state.

The E-match is not fired upon entering this state.

Written by Electronics Team, Imperial College London Rocketry
*/

#include "Arduino.h"
#include "state.h"
#include "stateMachine.h"

#ifndef CALIBRATION_H
#define CALIBRATION_H

class Calibration: public State {
  public:
    void initialise();
    State* update();
    int8_t stateID = 1;
};

#endif
