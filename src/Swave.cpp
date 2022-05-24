/* SWAVE.CPP
    The swaveClass defined in this module implements a standing wave function that can be used to modulate LED brightness. The 
    wave characteristics are defined in terms of wavelength, frequency, and amplitude.  The "origin" of the standing wave is at
    position x = 0; this is a standing wave node where the wave value (y) is always 0. The first antinode, where the wave value is
    equal to the specified amplitude, occurs at x = (wavelength / 4). The value(position) function is used to
    evaluate the wave at any position (in mm) where position >= 0, and the returned value is in the range -ampltude to +amplitude. 
    
    The swaveClass object also includes a rampClass object that is optionally used to further scale the  wave output. The ramp effect 
    total duration (ramp-up, hold, ramp-down) is automatically set to the same duration as the wave, although the ramp-up and ramp-down 
    durations must be separately configured with swaveClass::setRamp(). 
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "Swave.h"


/* swaveClass::start()
    Starts the sine wave modulation effect. 
  Parameters:
    float duration: Total duration of the effect (in seconds). Duration = 0 results in an infinite (non-terminating) effect
    float frequency: Sine wave frequency (in Hz). Can be positive or negative
    float ampl: Defines the maximum output of the val() function
  Returns: None
*/
void swaveClass::start(float duration, float wavelen, float freq, float ampl) {
  amplitude = constrain(ampl, 0, 1);  // make sure parameter is in legal range
  waveLength = wavelen;
  if (duration == 0)  // if infinite duration
    effectSteps = 0;    // signal to step()
  else
    effectSteps = ComputeSteps(duration);  // total number of steps in wave effect
      // angle change per step; Negative angle delta makes travelling wave move in "positive" direction
  phaseDelta = -(TWO_PI * freq * stepPeriod);
  phaseAngle = 0;
  stepNum = 0;
  active = true;
  ramp.start(duration);   // start ramp with same duration
}


/* swaveClass::setRamp() 
    Pass-through to rampClass::setRamp (for convenience). Sets the duration of each of the ramp-up and ramp-down phases of the 
    trapezoidal ramp function to be applied in subsequent calls to swaveClass::start()
  Parameters:
    float rampDur: Duration of each of the ramp-up and ramp-down phases
  Returns: None
*/
void swaveClass::setRamp(float rampDur) {
  ramp.setRamp(rampDur);
}


/* swaveClass::step() 
    Called once per setp period (frame) to update effect, if active
  Parameters: None
  Returns: None
*/
void swaveClass::step() {
  if (active) {
    phaseAngle += phaseDelta;
    ramp.step();  // update ramp function (if active)
    if (effectSteps > 0) {  // if finite duration
      stepNum++;
      if (stepNum >= effectSteps) // duration is over
        active = false;
    }
  }
}


/* swaveClass::value() 
    Returns the current standing wave value (amplitude-scaled) at the specified distance from the wave origin, also scaled by the 
    embedded ramp function. A value of 0 will be returned prior to the first call to swaveClass::step().
    The ramp will have no effect unless swaveClass::setRamp() is used to set a non-zero ramp-up/down duration.
  Parameters: 
    float position: Distance (mm) from the wave origin (x = 0)
  Returns: 
    float: Current standing wave value at the specified position (distance from origin), in the range (-amplitude to +amplitude)
*/
float swaveClass::value(float position) {
  float retVal;

  if (active) {
    retVal = sin(TWO_PI * (position / waveLength)) * cos(phaseAngle) * amplitude;
    retVal *= ramp.val; // scale by current ramp function value
    return (retVal);
  }
  else
    return (0);   // wave output is always 0 when inactive
}


