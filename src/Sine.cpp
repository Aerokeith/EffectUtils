/* SINE.CPP
    The sineClass defined in this module implements a level-shifted (offset) sine wave. The amplitude of the unshifted sine wave 
    is specified as a single-sided absolute value (0 - 1), meaning that the peak-to-peak amplitude can be up to 2. The offset 
    can be in the range 0 - 1. The value() function returns the current offset sine wave value, clipped to the range 0 - 1. Example:
    offset = 0.5, level = 0.5 produces a full-range  sine wave with no clipping. 
    A rampDur parameter value of > 0 indicates that the specified parameter values (frequency, level, amplitude) are to be ramped from 
    their current value to the specified new values over the ramp duration. RampDur=0 causes the specified parameters to be applied
    immediately, and this can be used to establish initial conditions for a subsequent ramp to otrher values. 
    Once started by a call to sineClass::update(), there are only two ways to terminate this effect:
      1. update() is called with frequency=0. The effect is terminated (becomes inactive) immediately
      2. update() is called with amplitude=0. The effect remains active until the sine wave amplitude is ramped down to 0, based on the 
          time specxified in rampDur. 
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "RampVar.h"
#include "Sine.h"


/* sineClass::update()
    Starts the sine wave modulation effect, or updates the parameters of the effect if it is already active. 
  Parameters:
    float level: target offset of sine wave baseline from 0 (0 - 1)
    float frequency: Sine wave frequency (in Hz)
    float ampl: Defines the maximum absolute value of the sine wave (0 - 1) prior to the offset being applied
    float rampTime: Duration of ramp from the previous sine wave parameters to the new values specified by the 3 parameters above.
                      A value of 0 causes the other parameters to be applied immediately.
  Returns: None
*/
void sineClass::update(float freq, float level, float ampl, float rampDur) {
  if ((!active) || (rampDur == 0)) {  // is sine effect hasn't been started yet, or if parameters have immediate effect
    active = true;
    phaseAngle = 0;
    phaseDelta = (TWO_PI * freq * stepPeriod);
    amplitude = ampl;
    offset = level;
    frequency = freq;
    freqRamp.active = false;
    offsetRamp.active = false;
    amplitudeRamp.active = false;
  } 
  if (rampDur > 0) {    // parameters are to be "ramped in"
    freqRamp.start(&frequency, freq, rampDur);
    offsetRamp.start(&offset, level, rampDur);
    amplitudeRamp.start(&amplitude, ampl, rampDur);
  }
}


/* sineClass::step() 
    Called once per step period (frame) to update effect
  Parameters: None
  Returns: None
*/
void sineClass::step() {
  if (active) {
    freqRamp.step();
    offsetRamp.step();
    amplitudeRamp.step();
    phaseDelta = (TWO_PI * frequency * stepPeriod);   // recompute in case it was being ramped
    phaseAngle += phaseDelta;
  }
}


/* sineClass::value() 
    Returns the current value of the level-offset sine wave, at a specified phase angle offset. This can be used to obtain the 
      sine wave value at different positions along a linear fixture.
  Parameters:
    float phaseOffsetFrac: Additonal phase angle offset (from sineClass::phaseAngle), specified as a fraction of a complete
        cycle (0 - 1)
  Returns: 
    float: current offset sine wave value, clipped to the range 0 - 1
*/
float sineClass::value(float phaseOffsetFrac) {
  float phaseOffset;
  float retVal;

  phaseOffset = TWO_PI * phaseOffsetFrac;
  if (active) {
    retVal = constrain((sin(phaseAngle + phaseOffset) * amplitude) + offset, 0, 1);
    if (amplitude == 0)   // if the amplitude has been ramped down to 0
      active = false;     // terminate this effect
    return (retVal);
  }
  else
    return (0);   // wave output is always 0 when inactive
}


/* sineClass::value() 
    Returns the current value of the level-offset sine wave, with no phase offset. Overload of function above
  Parameters: None
  Returns: 
    float: current offset sine wave value, clipped to the range 0 - 1
*/
float sineClass::value() {
  return value(0);    // no phase offset
}
