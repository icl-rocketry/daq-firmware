/* 
Code used to process states, and the transitions between them

Written by the Electronics team, Imperial College London Rocketry
*/

#include "Arduino.h" 
#include "stateMachine.h"
#include "states/state.h"

bool stateMachine::initialise(State* initStatePtr) {
  changeState(initStatePtr);
}

void stateMachine::update() {
  State* newStatePtr = _currStatePtr -> update();

  if (newStatePtr != _currStatePtr) {
    changeState(newStatePtr);
  }
}

void stateMachine::changeState(State* newStatePtr) {
  _currStatePtr = newStatePtr;
  _currStatePtr -> initialise();
}
