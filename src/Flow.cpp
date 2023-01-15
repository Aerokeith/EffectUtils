/* FLOW.CPP
    This module defines the flowClass, which implements a linear ramp function that "flows" across a specified distance over a specified
    duration. The ramp width is also specified, and will typically be less than or equal to the flow distance. 
    The function val() is used to access the amplitude of the ramp at a specified offset from the flow origin. The ramp val() ranges 
    from 0 to 1, such that val(0) == 0 at time 0 and ramps ups to val(0) == 1 as the upper edge of the ramp reaches the flow origin.
    A completed() function is provided to detect when a flow effect is completed (just became inactive).
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "Flow.h"


/* flowClass::start()
    Starts the flow function function
  Parameters: 
    float duration: Total duration of the flow effect
    float distance: Total distance (in mm) that the leading edge of the flow ramp will move in the specified duration (determines the flow speed)
    float rampLen: Length of the linear ramp (in mm) that starts at the flow origin (distance = 0)
  Returns: None
*/
void flowClass::start(float duration, float distance, float rampLen) {
  effectSteps = ComputeSteps(duration);  // total number of steps in fade effect
  deltaDist = distance / (float) effectSteps;   // compute "speed" of flow leading edge (mm/step)
  rampWidth = constrain(rampLen, 1, distance);    // ramp width must be > 0 and <= distance
  rampSlope = (float) 1.0 / rampWidth;   // = delta-value / delta-distance
  curPos = 0;
  completedFlag = false;
  stepNum = 0;
  active = true;
}


/* flowClass::step()
    Called once per step period (frame) to update flow effect, if active
  Parameters: None
  Returns: None
*/
void flowClass::step() {
  if (active) {
    curPos += deltaDist;
    stepNum++;
    if (stepNum >= effectSteps) {  // if flow is done
      completedFlag = true;
      active = false;
    }

  }
}


/* flowClass::val()
    Returns the value of the flow (linear ramp) function (in range 0 - 1) at a specified offset from the flow origin, in the direction 
    of the flow. If the offset is greater than the current position of the ramp leading edge, a value of 0 is returned. If the offset
    if less than or equal to the position of the leading edge, a value of between 0 and 1 is returned. 
  Parameters: 
    float offset: Position (in mm) relative to the origin and direction of the flow. 
  Returns: None
*/
float flowClass::val(float offset) {
  float rampPos;

  if (!active)
    return (0.0f);
  rampPos = curPos - offset;  // find distance from start of ramp to specified offset
  if ((rampPos <= 0) || (offset < 0))   // if the ramp hasn't reached the offset position
    return (0.0f);
  if (rampPos >= rampWidth)   // if the ramp has fully passed the offset position
      return (1.0f);
  return (rampPos * rampSlope);   // offset is somewhere inside the ramp
}


/* flowClass::completed()
    Returns true when a previously-active flow effect has become inactive (i.e. just finished). Subsequent calls to completed()
    return false until a new flow effect is started. 
    rampSlope. 
  Parameters: None
  Returns: 
    bool: True when a previously-active flow effect has become inactive (i.e. just finished).
*/
bool flowClass::completed() {
  bool retVal;

  retVal = completedFlag;
  completedFlag = false;
  return retVal;
}