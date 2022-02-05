/* WAVELET.CPP
    The waveletClass defined in this module implements an effect that "launches" a number of discrete sine wave-shaped "wavelets"
    over a specified distance, where each wavelet has the same target speed and acceleration. The nominal length of each wavelet 
    (a single sine wave cycle) is specified, but a random variation is applied to the length of each wavelet. Similarly, a nominal 
    inter-wavelet delay is specified, but a random variation in the delay is applied. Default levels of variation (fractions of nominal) 
    are defined, but may be changed. The maximum number of wavelet that can be simultaneously active is defined by a constant 
    (currently 8). 
*/

#include <Arduino.h>
#include "MotionUtils.h"
#include "Wavelet.h"

//char vstr[80];  // DEBUG


/* waveletClass::start()
    Starts the wavelet effect. 
  Parameters:
    float duration: Total duration of the effect (in seconds). Duration = 0 results in an infinite (non-terminating) effect
    float dist: Total distance (mm) over which the effect is applied (only affects the val() function)
    float speed: Target speed (mm/sec) of each wavelet
    float accel: Acceleration (mm/sec/sec) used to ramp up to the target speed
    float len: Nominal length (mm) of each wavelet
    float delay: Nominal delay between each wavelet launch
  Returns: None
*/
void waveletClass::start(float duration, float dist, float speed, float accel, float len, float delay) {
  effectSteps = ComputeSteps(duration);
  distance = dist;
  maxVelocity = speed * stepPeriod; // convert to mm/step
  acceleration = accel * stepPeriod * stepPeriod; // convert to mm/step/step
  nomLength = len;
  nomDelay = delay;
  for (uint8_t w = 0; w < WAVELETS_MAX_NUM; w++) {
    wvl[w].active = false;
  }
  stepNum = 0;
  nextWvl = 0;  // prepare to launch wavelet 0
  lastWvl = 0;  // (lastWvl == nextWvl) means no wavelets launched yet
  launchStep = 0; // launch first wavelet with no delay
  active = true;
}


/* waveletClass::step() 
    Called once per setp period (frame) to update effect, if active
  Parameters: None
  Returns: None
*/
void waveletClass::step() {
  uint8_t w;

  if (active) {   // if the wavelet effect is active
    for (w = 0; w < WAVELETS_MAX_NUM; w++) {   // for each possible wavelet
      if (wvl[w].active) {          // if this wavelet is active
        if (wvl[w].velocity < maxVelocity)  // if haven't reached max velocity
          wvl[w].velocity = min(maxVelocity, wvl[w].velocity + acceleration);   // accelerate until max velocity reached
        wvl[w].position += wvl[w].velocity;   // move it at the defined speed
        if (wvl[w].position > (distance + (wvl[w].length / 2))) {   // if wavelet is done
          wvl[w].active = false;
        }
      }
    }
    if (stepNum >= launchStep) { // if it's time to launch a new wavelet
      launch();
        // compute time (step) for next launch
      launchStep = stepNum + (uint16_t) (randomVar(nomDelay, delayVar) / stepPeriod);
    } 
    if (effectSteps > 0) {    // if finite effect duration
      stepNum++;
      if (stepNum >= effectSteps) 
        active = false;
    }
  } 
}


/* waveletClass::launch() 
    Launches a new wavelet, unless the maximum number if already active. Note that wavelets are launched in round-robin sequence
    from the array of waveletStruct, since each wavelet takes about the same amount of time to reach the maximum distance and
    be de-activated. 
  Parameters: None
  Returns: None
*/
void waveletClass::launch() {
  if (!wvl[nextWvl].active) {   // if wavelet is available to be launched
    wvl[nextWvl].length = randomVar(nomLength, lengthVar);  // apply randonm length variation
    wvl[nextWvl].velocity = maxVelocity * WAVELET_START_VELOCITY;  // start wavelet at fraction of target velocity
    wvl[nextWvl].position = -(wvl[nextWvl].length / 2); // start wavelet just off "left" edge
    wvl[nextWvl].active = true;
    lastWvl = nextWvl;  // this new wavelet now becomes the last one launched
    nextWvl = (nextWvl + 1) % WAVELETS_MAX_NUM; // move to next member of array, with wrap-around
  }
}


/* waveletClass::randomVar() 
    Applies a random variation to a nominal parameter value. The random variation is specified as a fraction with a minimum value
    of >0. The return value will be the nominal value +/- (up to) the specified fraction. For example, randomVar(10, 0.2) will return a 
    random float value in the range 8.0 to 12.0 inclusive. 
  Parameters: 
    float nomVal: Nominal value to be varied
    float maxVar: Maximum variation as a fraction of the nominal value, in range 0.0 to 1.0 exclusive
  Returns:
    float: randomized value
*/
float waveletClass::randomVar(float nomVal, float maxVar) {
  int maxVarI;
  float randVar;

  maxVarI = maxVar * 100; // convert fraction to integer in range 0 - 100, exclusive
    // double the range, randomize, and then re-normalize
  randVar = ((float) random((maxVarI * 2) + 1) / 100) - maxVar;
  return (nomVal * (1.0 + randVar));
}


/* waveletClass::val() 
    Returns the value of the wavelet sine function (in range 0 - 1.0) at the specified offset position from 0. The return value
    will be 0 of the position is not within the distance specified with start(), or if no wavelet sinusoids are currently passing
    through the specified position. Note that the wavelet sine fucntions are shifted and downscaled up so that their value is 0 at 
    both the left and right edges of the wavelet, and 1.0 at the wavelet peak.  
  Parameters: 
    float pos: Position offset across the distance specified by start(), i.e. in the range 0 - distance
  Returns: 
    float: sine value (shifted/scaled to range 0 - 1) of any wavelet intersecting the specified position
*/
float waveletClass::val(float pos) {
  float retVal;
  float offset;
  float angle;

  if (!active || (pos < 0) || (pos > distance)) 
    return (0);
  retVal = 0;
  for (uint8_t w = 0; w < WAVELETS_MAX_NUM; w++) {  // check all wavelets
    if (wvl[w].active) {  // bt only if wavelet is active
      offset = pos - wvl[w].position;   // get offset from pos to wavelet center
      if (abs(offset) <= (wvl[w].length / 2)) {   // if offset <= half of wavelength
        angle = ((offset / wvl[w].length) * TWO_PI) + (PI / 2); // compute phase angle at offset
        retVal += (sin(angle) + 1) / 2; // shift up and scale down
        retVal = min(retVal, 1.0);
      }
    }
  }
  return (retVal);
}
