/* RAMPVAR.CPP
  This module defines the rampVarClass, which implements an object that will ramp a pointer-specified float variable from its
  current value to a specified target (end) value in a specified duration. 
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "RampVar.h"


/* rampVarClass::start()
    Starts the rampVar effect. 
  Parameters:
    float *var: Pointer to the variable being ramped
    float rampDur: Duration  of the ramp (in seconds) from the current variable value to targetVal
    float targetVal: final value of the ramped variable at the end of the rampDur
  Returns: None
*/
void rampVarClass::start(float *var, float targetVal, float rampDur) {

  varPtr = var;
  endVal = targetVal;
  effectSteps = ComputeSteps(rampDur);  // total number of steps in fade effect
  rampDelta = (endVal - *varPtr) / effectSteps;
  stepNum = 0;
  active = true;
}


/* rampVarClass::step()
    Called once per step period (frame) to update effect, if active
  Parameters: None
  Returns: None
*/
void rampVarClass::step() {
  if (active) {   // if ramp is currently active (started, not finished)
    *varPtr += rampDelta;
    stepNum++;
    if (stepNum >= effectSteps) {  // if fade is done
      *varPtr = endVal;  // correct any potential rounding errors to ensure variable ends up at targetVal (specified in start())
      active = false;
    }
  }
}
