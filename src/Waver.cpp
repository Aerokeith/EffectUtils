/* WAVER.CPP
    This module defines two classes: waveClass and rampClass, which can be used separately or in conjunction. The waveClass 
    implements a single sine wave function that can be configured using two diferent methods. The first method specifies a duration,
    (seconds), frequency (Hz) and maximum amplitude (between 0 and 1). The second method is equivalent, but for convenience defines 
    the sine frequency in terms of wavelength (mm) and speed (mm/sec); i.e. frequency = speed/wavelength. Once started, the sine wave
    is available using the val() function, where the return value is in the range -amplitude to +amplitude. The 
    overload function val(offset) can also be used, where offset (0 to 1) specifies a phase angle offset as a fraction of a complete cycle
    (2*π radians). Note that val(0) and val(1) will return the same value when called during the same step (frame). 

    For example, a "shimmer" function involving N LEDs can be implemented using a single waveClass object. Each LED is assigned a random
    offset value that is used in each subsequent call to val(offset). The returned sine value is used to modulate the brightness of each
    LED at the same frequency, while the random offsets mitigate any obvious synchronization. 

    Another example is a "travelling wave" effect used to modulate a strip of equally-spaced LEDs with a sine wave that appears to move
    down the strip. In this case the wavelength and speed are defined based on the LED spacing and the desired dimensions of the effect. 
    For each frame, the sine value for each LED is obtained using a phase angle offset that is based on the LEDs position (relative to an
    arbitrary origin point) as a fraction of the sine wavelength. 

    The rampClass implements a single trapezoidal ramp function where the ramp-up and -down durations are equal. The class variable val is 
    used to access the current ramp function value, which is 0 at the ramp start, 1 during the hold phase, and 0 again at the end of the 
    ramp-down. The ramp value is typically used to scale the output of some other effect, to provide a smooth transition into and out of 
    the effect. To this end, the waveClass object includes a rampClass object that is optionally used to further scale the sine wave
    output. The ramp total duration is automatically set to the same duration as the wave, although the ramp-up and ramp-down durations
    must be separately configured with waveClass::setRamp().

    Both the waveClass and rampClass objects include one or more start() methods, as well as a step() function that must be called once
    per effect step period, defined by calling setStep(). The waveClass::step() fuunction includes a call to 
    rampClass::step() for the embedded rampClass object. 
*/
#include <Arduino.h>
#include "Waver.h"


/* waveClass::waveClass()
    Object constructor
  Parameters: None
  Returns: None
*/
waveClass::waveClass() {
  active = false;   
  stepPeriod = 0.01;  // set default period of 10ms (in case setStep() isn't called)
}


/* waveClass::setStep()
    Sets the wave step period to be used by waveClass::start(). Also sets the (same) period for the embedded ramp function
  Parameters: 
    float stepPerMs: Step duration in integer milliseconds
  Returns: None
*/
void waveClass::setStep(uint16_t stepPerMs) {
  stepPeriod = (float) stepPerMs / 1000;
  ramp.setStep(stepPerMs);
}


/* waveClass::start()
    Starts the sine wave modulation effect. 
  Parameters:
    float duration: Total duration of the effect (in seconds). Duration = 0 results in an infinite (non-terminating) effect
    float frequency: Sine wave frequency (in Hz). Can be positive or negative
    float ampl: Defines maximum range of val() function as -ampl to +ampl
  Returns: None
*/
void waveClass::start(float duration, float frequency, float ampl) {
  amplitude = constrain(ampl, 0, 1);  // make sure parameter is in legal range
  if (duration == 0)  // if infinite duration
    waveSteps = 0;    // signal to step()
  else
    waveSteps = (uint16_t) ceil(duration / stepPeriod);  // total number of steps in wave effect
      // angle change per step; Negative angle delta makes travelling wave move in "positive" direction
  phaseDelta = -(TWO_PI * frequency * stepPeriod);
  phaseAngle = 0;
  stepNum = 0;
  active = true;
  ramp.start(duration);   // start ramp with same duration
}


/* waveClass::start() [Overload]
    Starts the sine wave modulation effect with an alternateset of parameters
  Parameters:
    float duration: Total duration of the effect (in seconds). Duration = 0 results in an infinite (non-terminating) effect
    float wavelen: Wavelength (in mm)
    float speed: Speed of travelling wave (mm/sec)
    float ampl: Defines maximum range of val() function as -ampl to +ampl
  Returns: None
*/
void waveClass::start(float duration, float wavelen, float speed, float ampl) {
  waveLength = max(wavelen, 1); // prevent divide by 0
  start(duration, (speed / waveLength), ampl);
}


/* waveClass::setRamp() 
    Pass-through to rampClass::setRamp (for convenience). Sets the duration of each of the ramp-up and ramp-down phases of the 
    trapezoidal ramp function to be applied in subsequent calls to waveClass::start()
  Parameters:
    float rampDur: Duration of each of the ramp-up and ramp-down phases
  Returns: None
*/
void waveClass::setRamp(float rampDur) {
  ramp.setRamp(rampDur);
}


/* waveClass::step() 
    Called once per FRAME_PERIOD to update effect, if active
  Parameters: None
  Returns: None
*/
void waveClass::step() {
  if (active) {
    phaseAngle += phaseDelta;
    ramp.step();  // update ramp function (if active)
    if (waveSteps > 0) {  // if finite duration
      stepNum++;
      if (stepNum >= waveSteps) // duration is over
        active = false;
    }
  }
}


/* waveClass::val() 
    Returns the current sine wave value (amplitude-scaled) at the specified phase offset, also scaled by the embedded ramp
    function. A value of 0 will be returned prior to the first call to waveClass::step(), e.g. sin(0) * amplitude = 0.
    The ramp will have no effect unless waveClass::setRamp() is used to set a non-zero ramp-up/down duration.
  Parameters: 
    float offset: Fractional part specifies the phase offset as a fraction of a complete cycle (2π radians). Integer part has no effect
  Returns: 
    float: Current sine wave value at the specified phase offset, in the range (-amplitude to +amplitude)
*/
float waveClass::val(float offset) {
  float retVal;

  if (active) {
    retVal = sin(phaseAngle + (offset * TWO_PI)) * amplitude;
    retVal *= ramp.val; // scale by current ramp function value
    return (retVal);
  }
  else
    return (0);   // wave output is always 0 when inactive
}


/* waveClass::val() [Overload]
    Alternate method when phase offsets aren't used (e.g. single sine-modulated LED)
  Parameters: 
    float offset: Fractional part specifies the phase offset as a fraction of a complete cycle (2π radians). Integer part has no effect
  Returns: 
    float: Current sine wave value at phase offset = 0, in the range (-amplitude to +amplitude)
*/
float waveClass::val() {
  return (val(0));
}




/* rampClass::rampClass()
    Object constructor
  Parameters: None
  Returns: None
*/
rampClass::rampClass() {
  rampTime = 0;   // default is no ramp until explicitly set by setRamp() or start()
  stepPeriod = 0.01;  // set default period of 10ms (in case setStep() isn't called)
  active = false;
}


/* rampClass::setStep()
    Sets the ramp step period to be used by rampClass::start(). 
  Parameters: 
    float stepPerMs: Step duration in integer milliseconds
  Returns: None
*/
void rampClass::setStep(uint16_t stepPerMs) {
  stepPeriod = (float) stepPerMs / 1000;
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
    rampSteps = (uint16_t) ceil(rampTime / stepPeriod);  // number of FRAME_PERIOD steps in up or down ramp
    holdSteps = 0;  // hold phase has infinite duration after ramp up
  }
  else {  // finite duration wave and ramp
    rampTimeNow = constrain(rampTime, 0, (duration / 2));  // force ramp duration to be no more than 1/2 the total duration
    rampSteps = (uint16_t) ceil(rampTimeNow / stepPeriod);  // number of FRAME_PERIOD steps in up or down ramp
    totalSteps = (uint16_t) ceil(duration / stepPeriod); // total # steps including ramps
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
    Called once per FRAME_PERIOD to update effect, if active
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
