/* 
Code used to process states, and the transitions between them

Written by the Electronics team, Imperial College London Rocketry
*/

#include "Arduino.h" 
#include "stateMachine.h"

stateMachine::stateMachine() {
}

bool stateMachine::init(state * initStatePtr) {
  changeState(initStatePtr);
}

void stateMachine::update() {
  _currStatePtr -> update();
}

void stateMachine::changeState(state * newStatePtr) {
  _currStatePtr = newState;
  _currStatePtr - > init;
}

