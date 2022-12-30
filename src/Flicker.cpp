/* FLICKER.CPP
    This module defines flickerClass, which implements a filtered random-step effect that can be used to simulate flame/flicker-type
    effects. The current value of the flicker function is accessed with the flickerClass:val() function, which ranges from 0 to 1.0. 
    The flicker is applied at a constant (specified) frequency where target flicker output level is randomly chosen for each cycle.
    A specified filter value constrains the maximum per-cycle change from the current level in the direction of the new target level,
    enabling control over the "smoothness" of the flciker effect. The flickerClass::val() can be used to modulate the brightness of 
    one or more LEDs, or can be used to control the number of consecutive LEDs in a strip that are turned on. The flicker effect includes
    an embedded ramp function that can be used to fade in/out the flicker effect. 
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "Flicker.h"


/* setRamp()
    Sets the duration of the embedded ramp function (set Ramp.cpp) that is used to gradually increase, hold, and then decrease
    the flicker effect output (based on flickVal) over the duration of the effect.
  Parameters: 
    float rampUpDur: duration of each of the ramp-up phases (seconds)
    float rampDownDur: duration of each of the ramp-down phases (seconds)
  Returns: None
*/
void flickerClass::setRamp(float rampUpDur, float rampDownDur) {
  ramp.setRamp(rampUpDur, rampDownDur);
}


/* setRamp() [Overload]
    Sets the duration of the embedded ramp function (set Ramp.cpp) that is used to gradually increase, hold, and then decrease
    the flicker effect output (based on flickVal) over the duration of the effect.
  Parameters: 
    float rampDur: duration of each of the ramp-up and ramp-down phases (seconds)
  Returns: None
*/
void flickerClass::setRamp(float rampDur) {
  ramp.setRamp(rampDur);
}


/* start()
    Starts the flicker effect with the specified parameters. The embedded ramp function is also started, but this will have no effect
      unless setRamp() has been called with a non-zero rampTime.
  Parameters: 
    float duration: Total effect duration (seconds). Duration = 0 specifies an infinite duration
    float frequency: Frequency of the flicker (Hz)
    float filter: Value between 0 and 0.99999 that specifies a constraint on the slew rate of flickVal in response to a new flicker
                  cycle targetVal. filter = 0 specifies no filtering (fastest response)
    float minVal: Specifies a minimum value (0 - 1) of flickerClass::flickVal prior to scaling by the ramp function
  Returns: None
*/
void flickerClass::start(float duration, float frequency, float filter, float minVal) {
  effectSteps = ComputeSteps(duration);
  frequency = max(frequency, 0.01); 
  maxDelta = (float) 1.0 - filter;  // maxDelta = 1.0 when (filter == 0)
  cycleSteps = ComputeSteps((float) 1.0 / frequency);
  minTarget = (uint32_t) (minVal * 100);  // compute min value to use in random() function in step()
  stepNum = 0;
  cycleStepNum = 0;
  flickVal = minVal + ((1 - minVal) / 2);   // start at 50% full scale
  ramp.start(duration);   // start the embedded ramp
  active = true;
}


/* step()
    Called once per step period to update the flicker effect (if active).
  Parameters: None
  Returns: None
*/
void flickerClass::step() {
  float delta;

  if (active) {
    if (cycleStepNum == 0) {  // if beginning of new cycle
      targetVal = (float) random(minTarget, 101) / 100;  // get random value between 0.0 and 1.0
    }
    delta = targetVal - flickVal;  
    if (delta > 0) {                      // new targetVal is > current flickVal
      flickVal += min(delta, maxDelta);   // add delta to get there, constrained by maxDelta
    }
    else if (delta < 0) {   // new targetVal < current flickVal
      flickVal += max(delta, -maxDelta);  // add (negative) delta, constrained by -maxDelta
    }
    ramp.step();    // update the embedded ramp function
    cycleStepNum++;
    if (cycleStepNum >= cycleSteps) {   // if cycle is done
      cycleStepNum = 0; // start new cycle in next step
    }
    if (effectSteps > 0) {  // if finite duration
      stepNum++;
      if (stepNum >= effectSteps) {
        flickVal = 0;
        active = false;
      }
    }
  }
}

/* val()
    Returns the current flickerVal, scaled by the embedded ramp function value (if enabled).
  Parameters: None
  Returns: 
    float: Ramp-modulated flicker function output
*/
float flickerClass::val() {
  if (active) {
    return (flickVal * ramp.val);
  }
  else 
    return (0);
}
