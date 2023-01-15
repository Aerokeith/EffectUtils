/* WAIT.CPP
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "Wait.h"


/* waitClass::start()
    Starts the wait "effect"
  Parameters: 
  Returns: None
*/
void waitClass::start(float duration) {
  effectSteps = ComputeSteps(duration);  // total number of steps in wait effect
  completedFlag = false;
  stepNum = 0;
  active = true;
}


/* waitClass::step()
    Called once per step period (frame) to update wait effect, if active
  Parameters: None
  Returns: None
*/
void waitClass::step() {
  if (active) {
    stepNum++;
    if (stepNum >= effectSteps) {  // if wait is done
      completedFlag = true;
      active = false;
    }
  }
}


/* waitClass::completed()
    Returns true when a previously-active wait effect has become inactive (i.e. just finished). Subsequent calls to completed()
    return false until a new wait effect is started. 
  Parameters: None
  Returns: 
    bool: True when a previously-active wait effect has become inactive (i.e. just finished).
*/
bool waitClass::completed() {
  bool retVal;

  retVal = completedFlag;
  completedFlag = false;
  return retVal;
}