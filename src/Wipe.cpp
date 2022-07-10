/* WIPE.CPP
    This module defines the wipeClass, which implements a moving line that "wipes" across a 2-dimensional plane. The line is specified
    by the 2D coordinates of a point on the line and the angle of the line in the global coordinate system. The line wipes in the 
    directional perpendicular to the line, covering a specified distance in a specified duration. The line represents the leading edge of
    a ramp function that increases in value from 0.0 on the line to 1.0 at a specified distance (ramp width) behind the wipe line.

    The value(x, y) function is used to access the ramp function value at the point specified by (x, y):
      value(x, y) = 0 If the wipe line has not yet crossed the point (x, y)
      0 < value(x, y) < 1 if the line has crossed the point but the distance between the line and point is less than the ramp width
      value(x, y) = 1 if the trailing edge (top) of the ramp has crossed the point (x, y)
    
    A completed() function is provided to detect when a wipe effect is completed (just became inactive).
*/
#include <Arduino.h>
#include "EffectUtils.h"
#include "Wipe.h"


/* wipeClass::start()
    Starts the wipe function
  Parameters: 
    float duration: Total duration of the wipe effect
    coordStruct refPos: Coordinates of a point on the wipe line
    float angle: Angle (degrees) of the wipoe line in gthe global coordinate system
    float distance: Total distance (in mm) that the leading edge of the wipe ramp will move in the specified duration (determines the wipe speed)
    float rampLen: Length of the linear ramp (in mm) that starts at/behind the wipe line
  Returns: None
*/
void wipeClass::start(float duration, coordStruct refPos, float angle, float distance, float rampLen) {
  effectSteps = ComputeSteps(duration);  // total number of steps in wipe effect
  deltaDist = distance / (float) effectSteps;   // compute "speed" of wipe leading edge (mm/step)
  rampWidth = constrain(rampLen, 1, distance);    // ramp width must be > 0 and <= distance
  line.init(refPos, angle);   // initialize the wipe line
  completedFlag = false;
  stepNum = 0;
  active = true;
}


/* wipeClass::step()
    Called once per step period (frame) to update wipe effect, if active
  Parameters: None
  Returns: None
*/
void wipeClass::step() {
  if (active) {
    line.step(deltaDist);   // move the line based on the wipe distance / duration
    stepNum++;
    if (stepNum >= effectSteps) {  // if wipe is done
      completedFlag = true;
      active = false;
    }
  }
}


/* wipeClass::value()
    Returns the value of the flow (linear ramp) function (in range 0 - 1) at a specified offset from the flow origin, in the direction 
    of the flow. If the offset is greater than the current position of the ramp leading edge, a value of 0 is returned. If the offset
    if less than or equal to the position of the leading edge, a value of between 0 and 1 is returned. 
    
    Returns the ramp function value at the point specified by (x, y):
    value(x, y) = 0 If the wipe line has not yet crossed the point (x, y)
    0 < value(x, y) < 1 if the line has crossed the point but the distance between the line and point is less than the ramp width
    value(x, y) = 1 if the trailing edge (top) of the ramp has crossed the point (x, y)

  Parameters: 
    coordStruct pos: 2D coordinates (in mm) of a point in the global coordinate system. 
  Returns: 
    float: Value in range 0.0 - 1.0
*/
float wipeClass::value(coordStruct pos) {
  float distFromLine;

  if (!active)
    return (0.0f);
  distFromLine = line.distance(pos);
  if (distFromLine >= 0)   // line hasn't yet crossed the point
    return (0.0f);
  else {  // line has crossed point, so distance is negative
    distFromLine = -distFromLine;   // convert to positive value
    if (distFromLine > rampWidth)   // back end of ramp has already crossed the point
      return (1.0f);
    else  // point is in between wipe line and back of ramp
      return (distFromLine / rampWidth);    // return linear ramp value (0 - 1)
  }
}


/* wipeClass::completed()
    Returns true when a previously-active wipe effect has become inactive (i.e. just finished). Subsequent calls to completed()
    return false until a new wipe effect is started. 
  Parameters: None
  Returns: 
    bool: True when a previously-active wipe effect has become inactive (i.e. just finished).
*/
bool wipeClass::completed() {
  bool retVal;

  retVal = completedFlag;
  completedFlag = false;
  return retVal;
}