/* 
Code used to process states, and the transitions between them

Written by the Electronics team, Imperial College London Rocketry
*/

#include "Arduino.h" 
#include "stateMachine.h"

stateMachine::stateMachine() {
}

bool stateMachine::initialise(State * initStatePtr) {
  changeState(initStatePtr);
}

void stateMachine::update() {
  _currStatePtr -> update();
}

void stateMachine::changeState(State * newStatePtr) {
  _currStatePtr = newStatePtr;
  _currStatePtr -> initialise();
}

