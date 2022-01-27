/* WAVER.CPP
    The waveClass defined in this module implements a single sine wave function that can be configured using two diferent methods. 
    The first method specifies a duration, (seconds), frequency (Hz) and maximum amplitude (between 0 and 1). The second method is 
    equivalent, but for convenience defines the sine frequency in terms of wavelength (mm) and speed (mm/sec); i.e. 
    frequency = speed/wavelength. Once started, the sine wave is available using the val() function, where the return value is in 
    the range 0 to <amplitude>. The overload function val(offset) can also be used, where offset (0 to 1) specifies a phase angle 
    offset as a fraction of a complete cycle (2*π radians). Note that val(0) and val(1) will return the same value when called during 
    the same step (frame), since they both translate to the same phase angle offset.  

    For example, a "shimmer" function involving N LEDs can be implemented using a single waveClass object. Each LED is assigned a random
    offset value that is used in each subsequent call to val(offset). The returned sine value is used to modulate the brightness of each
    LED at the same frequency, while the random offsets mitigate any obvious synchronization. 

    Another example is a "travelling wave" effect used to modulate a strip of equally-spaced LEDs with a sine wave that appears to move
    down the strip. In this case the wavelength and speed are defined based on the LED spacing and the desired dimensions of the effect. 
    For each frame, the sine value for each LED is obtained using a phase angle offset that is based on the LEDs position (relative to an
    arbitrary origin point) as a fraction of the sine wavelength. 

    The waveClass object also includes a rampClass object that is optionally used to further scale the sine wave output. The ramp total 
    duration is automatically set to the same duration as the wave, although the ramp-up and ramp-down durations must be separately 
    configured with waveClass::setRamp().

    The step() function must be called once per effect step period, which is defined by calling setStep() prior to the first call to
    start(). Note that the waveClass::step() fuunction includes a call to rampClass::step() for the embedded rampClass object. 
*/
#include <Arduino.h>
#include "Waver.h"

//char wvstr[80]; // DEBUG

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
    float ampl: Defines the maximum output of the val() function
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
  phaseAngle = -(TWO_PI / 4); // sin(-π/s) is minimum point of wave, which results in val() = 0
  stepNum = 0;
  active = true;
  ramp.start(duration);   // start ramp with same duration
}


/* waveClass::start() [Overload]
    Starts the sine wave modulation effect with an alternateset of parameters
  Parameters:
    float duration: Total duration of the effect (in seconds). Duration = 0 results in an infinite (non-terminating) effect
    float wavelen: Wavelength (in mm)
    float speed: Speed of travelling wave (mm/sec). Spped can be negative to reverse wave direction.
    float ampl: Defines the maximum output of the val() function
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
    function. A value of 0 will be returned prior to the first call to waveClass::step().
    The ramp will have no effect unless waveClass::setRamp() is used to set a non-zero ramp-up/down duration.
  Parameters: 
    float offset: Fractional part specifies the phase offset as a fraction of a complete cycle (2π radians). Integer part has no effect
  Returns: 
    float: Current sine wave value at the specified phase offset, in the range (-amplitude to +amplitude)
*/
float waveClass::val(float offset) {
  float retVal;

  if (active) {
      // shift sin up to range 0 - 2, then scale to range 0 - 1, then scale by amplitude
    retVal = ((sin(phaseAngle + (offset * TWO_PI)) + 1) / 2) * amplitude;
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


