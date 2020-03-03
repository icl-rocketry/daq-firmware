/* 
Code used to process states, and the transitions between them

Written by the Electronics team, Imperial College London Rocketry
*/

#include "Arduino.h" 
#include "stateMachine.h"
#include "states/state.h"

void stateMachine::initialise(State* initStatePtr) {
  changeState(initStatePtr);
}

void stateMachine::update() {
  State* newStatePtr = _currStatePtr -> update();

  if (newStatePtr != _currStatePtr) {
    changeState(newStatePtr);
  }
  Serial.println(_currStatePtr -> getID());
}

void stateMachine::changeState(State* newStatePtr) {
  // Delete old state instance and change to new one
  delete _currStatePtr;
  _currStatePtr = newStatePtr;
  _currStatePtr -> initialise();
  Serial.print("CHANGED TO STATE: ");
  Serial.println(_currStatePtr -> getID());
}
