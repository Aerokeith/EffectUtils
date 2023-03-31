/* DROPLET.CPP
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "Droplet.h"


/* dropletClass::start()
    Starts the droplet effect
  Parameters: 
  Returns: None
*/
void dropletClass::start(float dist) {
  deltaDist = config->initVelocity * stepPeriod;    // convert initial velocity to distance per step (mm/step)
  accelDelta = config->acceleration * pow(stepPeriod, 2);  // convert accel (mm/sec^2) to (mm/step^2)
  tailLength = random(config->minTailLength, config->maxTailLength);  // compute random tail length
    // compute total distance for leading edge to travel so that the tail goes "off the end"
  distance = dist + config->headRampLen + config->headLength + tailLength;
  headSlope = 1 / config->headRampLen;      // droplet value() goes from 0 to 1 in length of head ramp
  tailSlope = 1 / tailLength;               // droplet value9) goes from 1 to 0 in length of tail ramp
  curPos = 0;
  completedFlag = false;
  active = true;
}


/* dropletClass::step()
    Called once per step period (frame) to update droplet effect, if active
  Parameters: None
  Returns: None
*/
void dropletClass::step() {
  if (active) {
    curPos += deltaDist;      // move current position based on current velocity (in mm/step)
    deltaDist += accelDelta;  // update velocity based on acceleration
    if (curPos >= distance) {  // if flow is done
      completedFlag = true;
      active = false;
    }

  }
}


/* droletClass::value()
    Returns the value of the flow (linear ramp) function (in range 0 - 1) at a specified offset from the flow origin, in the direction 
    of the flow. If the offset is greater than the current position of the ramp leading edge, a value of 0 is returned. If the offset
    if less than or equal to the position of the leading edge, a value of between 0 and 1 is returned. 
  Parameters: 
    float offset: Position (in mm) relative to the origin and direction of the flow. 
  Returns: None
*/
float dropletClass::value(float offset) {
  float relOffsetHead;  // relative offset from curPos (leading edge of head) to specified absolute offset
  float relOffsetTail;  // relative ofset from trailing edge of tail to specified absolute offset

  if (!active)
    return (0.0f);
  relOffsetHead = curPos - offset;  // find distance from leading edge (curPos) to specified offset
  if ((relOffsetHead <= 0) || (offset < 0))   // if the droplet leading edge hasn't reached the offset position
    return (0);
  relOffsetTail = (curPos - config->headRampLen - config->headLength - tailLength) - offset;  // find distance from trailing edge to offset
  if (relOffsetTail > 0)    // if the droplet tail has fully passed the offset position
    return (0);
  if (relOffsetHead < config->headRampLen)    // if offset is within leading edge ramp
    return (relOffsetHead * headSlope);       // return value based on leading edge ramp slope
  if (relOffsetHead < (config->headLength + config->headRampLen))   // if offset is within droplet head
    return (1.0);      // value() is 1.0 within head
  else              // else offset is witin the tail
    return (-relOffsetTail * tailSlope);
}


/* dropletClass::completed()
    Returns true when a previously-active flow effect has become inactive (i.e. just finished). Subsequent calls to completed()
    return false until a new flow effect is started. 
    rampSlope. 
  Parameters: None
  Returns: 
    bool: True when a previously-active flow effect has become inactive (i.e. just finished).
*/
bool dropletClass::completed() {
  bool retVal;

  retVal = completedFlag;
  completedFlag = false;
  return retVal;
}