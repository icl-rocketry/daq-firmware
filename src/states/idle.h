/*
State that the DAQ boots up into, where it is waiting for instruction from
the web interface

Written by Electronics Team, Imperial College London Rocketry
*/
#include "Arduino.h"
#include "state.h"

#ifndef IDLE_H
#define IDLE_H

class Idle: public State {
  public:
    void init();
    void update();
}

#endif
