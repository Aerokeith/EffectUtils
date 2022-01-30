/* RAMP.CPP
    This module defines the rampClass, which implements a single trapezoidal ramp function where the ramp-up and -down durations are equal. 
    The class variable val is used to access the current ramp function value, which is 0 at the ramp start, 1 during the hold phase, and 0
    again at the end of the ramp-down. The ramp value is typically used to scale the output of some other effect, to provide a smooth 
    transition into and out of the effect.
    The setStep() function must be called prior to the first call to start() to configure the duration of each ramp step. 
    The ramp-up/down durations (for all subsequent ramps) can be set using setRamp() or using one of the overloaded start() functions. 
*/
#include <Arduino.h>
#include "UtilConfig.h"
#include "Ramp.h"


/* rampClass::rampClass()
    Object constructor
  Parameters: None
  Returns: None
*/
rampClass::rampClass() {
  rampTime = 0;   // default is no ramp until explicitly set by setRamp() or start()
  active = false;
}


/* rampClass::setRamp()
    Sets the duration of each of the ramp-up and ramp-down phases of the trapezoidal ramp function to be applied
    in subsequent calls to rampClass::start(), unless the ramp duration is set in the call to start()
  Parameters: 
    float rampDur: duration of each of the ramp-up and ramp-down phases (seconds)
  Returns: None
*/
void rampClass::setRamp(float rampDur) {
  rampTime = abs(rampDur);   // ensure duration is positive
}


/* rampClass::start()
    Starts the trapezoidal ramp function
  Parameters: 
    float duration: Total duration of the ramp function, including all three phases (ramp-up, hold, ramp-down).
                    Duration = 0 specifies a ramp-up phase followed by an infinite-duration hold phase
  Returns: None
*/
void rampClass::start(float duration) {
  uint16_t totalSteps;  // number of FRAME_PERIOD steps
  float rampTimeNow;    // potentially-constrained ramp time for this start instance

  if (duration == 0) {  // if ramp has infinite duration
    rampSteps = (uint16_t) ceil(rampTime / STEP_PERIOD);  // number of STEP_PERIOD steps in up or down ramp
    holdSteps = 0;  // hold phase has infinite duration after ramp up
  }
  else {  // finite duration ramp
    rampTimeNow = constrain(rampTime, 0, (duration / 2));  // force ramp duration to be no more than 1/2 the total duration
    rampSteps = (uint16_t) ceil(rampTimeNow / STEP_PERIOD);  // number of STEP_PERIOD steps in up or down ramp
    totalSteps = (uint16_t) ceil(duration / STEP_PERIOD); // total # steps including ramps
    holdSteps = max((totalSteps - (2 * rampSteps)), 1); // remaining duration is hold period (ensure at least one hold step)
  }
  phase = ((rampSteps == 0) ? hold : rampUp); // start in rampUp phase unless no ramp steps
  stepNum = 0;
  rampDelta = 1.0 / rampSteps;  // delta to go from val=0 to val=1 by end of rampUp phase
  val = 0;  // ramp function initial value at start of rampUp
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
    setRamp(rampDur);
    start(duration);
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
        val += rampDelta;   // apply ramp-up step to output value
        stepNum++;
        if (stepNum >= rampSteps) {   // if ramp-up done
          stepNum = 0;
          val = 1.0;
          phase = hold;
        }
      break;
      case hold:
        val = 1.0;  //value stays at 1.0 during hold
        if (holdSteps > 0) {  // if hold duration isn't infinite
          stepNum++;
          if (stepNum >= holdSteps) { // if hold phase is done
            if (rampSteps == 0) {   // and if ramp is zero duration
              val = 0.0;
              active = false; // terminate ramp
            }
            else {  // hold is done and ramp is non-zero
              stepNum = 0;
              phase = rampDown;
            }
          }
        }
      break;
      case rampDown:
        val -= rampDelta;
        stepNum++;
        if (stepNum >= rampSteps) {
          val = 0.0;
          active = false;
        }
      break;
      default:
      break;
    }
  }
}
