/* 
Code used to process states, and the transitions between them

Written by the Electronics team, Imperial College London Rocketry
*/

#include "Arduino.h"

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

class stateMachine {
  public:
    stateMachine();
    bool init(state * initStatePtr);
    void update();
    void changeState(state * newStatePtr);

  private:
    state * _currStatePtr;
}

#endif
