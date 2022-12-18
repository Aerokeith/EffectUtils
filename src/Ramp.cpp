/* RAMP.CPP
    This module defines the rampClass, which implements a single trapezoidal ramp function where the ramp-up and -down durations are equal. 
    The class variable (float) val is used to access the current ramp function value, which is 0 at the ramp start, 1 during the hold phase, and 0
    again at the end of the ramp-down. The ramp value is typically used to scale the output of some other effect, to provide a smooth 
    transition into and out of the effect.
    The ramp-up/down durations (for all subsequent ramps) can be set using setRamp() or using one of the overloaded start() functions. 
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "Ramp.h"


/* rampClass::setRamp()
    Sets the duration of the ramp-up and ramp-down phases of the trapezoidal ramp function to be applied
    in subsequent calls to rampClass::start(), unless the ramp durations are set in the call to start()
  Parameters: 
    float rampUpDur: duration of the ramp-up phase (seconds)
    float rampDownDur: duration of the ramp-down phase (seconds)
  Returns: None
*/
void rampClass::setRamp(float rampUpDur, float rampDownDur) {
  rampUpTime = max(0, rampUpDur);
  rampDownTime = max(0, rampDownDur);
}


/* rampClass::setRamp() [Overload]
    Sets the duration of ramp-up and ramp-down phases to the same value
  Parameters: 
    float rampDur: duration of each of the ramp-up and ramp-down phases (seconds)
  Returns: None
*/
void rampClass::setRamp(float rampDur) {
  setRamp(rampDur, rampDur);
}



/* rampClass::start()
    Starts the trapezoidal ramp function
  Parameters: 
    float duration: Total duration of the ramp function, including all three phases (ramp-up, hold, ramp-down).
                    Duration = 0 specifies a ramp-up phase followed by an infinite-duration hold phase
    float rampUpDur: Duration of the ramp-up phase (seconds). Overrides the value in any previous call to setRamp()
    float rampDownDur: Duration of the ramp-down phase (seconds). Overrides the value in any previous call to setRamp()
  Returns: None
*/
void rampClass::start(float duration, float rampUpDur, float rampDownDur) {
  float rampTime;    // potentially-constrained ramp time for this start instance

  setRamp(rampUpDur, rampDownDur);    // set new values for rampClass::rampUpTime and rampClass::rampDownTime
  if (duration == 0) {  // if ramp has infinite duration
    rampUpSteps = ComputeSteps(rampUpTime);  // number of steps in ramp-up
    holdSteps = 0;  // hold phase has infinite duration after ramp up
  }
  else {  // finite duration ramp
    if (duration < (rampUpTime + rampDownTime)) {   // if duration is to short to accommodate full ramp-up/down
        // compute time at intersection of up and down ramps, constrained by total duration
      rampTime = ((-1/rampDownTime) * duration) / ((-1/rampDownTime) - (1/rampUpTime));
      rampUpSteps = ComputeSteps(rampTime); 
      rampDownSteps = ComputeSteps(duration - rampTime);  // remaining time used for down ramp
      holdSteps = 0;
  }
    else {  // duration is long enough to accommodate full ramp up/down
      rampUpSteps = ComputeSteps(rampUpTime);
      rampDownSteps = ComputeSteps(rampDownTime);
      holdSteps = ComputeSteps(duration - rampUpTime - rampDownTime);
    }
  }
  if (rampUpSteps == 0) {   // if no ramp-up phase
    val = 1.0;      // set val to max value
    phase = hold;   // go directly to hold phase
  }
  else {    // prepare to start ramp-up
    phase = rampUp;
    rampDelta = 1.0 / (float) rampUpSteps;  // delta to go from val=0 to val=1 by end of rampUp phase (if no time truncation)
    val = 0;  // ramp function initial value at start of rampUp
  }
  stepNum = 0;
  active = true;  
}


/* rampClass::start() [Overload]
    Starts the trapezoidal ramp function
  Parameters: 
    float duration: Total duration of the ramp function, including all three phases (ramp-up, hold, ramp-down).
                    Duration = 0 specifies a ramp-up phase followed by an infinite-duration hold phase
    float rampDur: Sets the duration of each of the ramp-up and ramp-down phases of the trapezoidal ramp function
  Returns: None
*/
void rampClass::start(float duration, float rampDur) {
    start(duration, rampDur, rampDur);  // specify equal-duration ramp-up and -down
  }


/* rampClass::start() [Overload]
    Starts the trapezoidal ramp function, using the default ramp durations, or those set by a previous call to setRamp()
  Parameters: 
    float duration: Total duration of the ramp function, including all three phases (ramp-up, hold, ramp-down).
                    Duration = 0 specifies a ramp-up phase followed by an infinite-duration hold phase
  Returns: None
*/
void rampClass::start(float duration) {
  start(duration, rampUpTime, rampDownTime);  // call start() using parameters from previously-set class variables
}


/* rampClass::step()
    Called once per step period (frame) to update effect, if active
  Parameters: None
  Returns: None
*/
void rampClass::step() {
  if (active) {
    switch (phase) {
      case rampUp:
        val = min(val + rampDelta, 1.0);   // apply ramp-up step to output value
        stepNum++;
        if (stepNum >= rampUpSteps) {   // if ramp-up done
          stepNum = 0;  // prep for next phase
          if (holdSteps > 0)  // if any steps in hold phase
            phase = hold;
          else  // otherwise skipo to ramp-down phase
            phase = rampDown;
        }
      break;
      case hold:
        if (holdSteps > 0) {  // if hold duration isn't infinite
          stepNum++;
          if (stepNum >= holdSteps) { // if hold phase is done
            if (rampDownSteps == 0) {   // and if ramp-down is zero duration
              val = 0.0;
              active = false; // terminate ramp
            }
            else {  // hold is done and ramp is non-zero
              stepNum = 0;
              phase = rampDown;
              rampDelta = (1.0 / (float) rampDownSteps);  // delta to go from val=1 to val=0 by end of rampDown phase (if no time truncation)
            }
          }
        }
      break;
      case rampDown:
        val = max(val - rampDelta, 0);  // apply ramp-down step to output value
        stepNum++;
        if (stepNum >= rampDownSteps) {
          val = 0.0;
          active = false;
        }
      break;
      default:
      break;
    }
  }
}
