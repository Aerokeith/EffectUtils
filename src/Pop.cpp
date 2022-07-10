#include "Arduino.h"
#include "EffectUtils.h"
#include "Pop.h"


void popClass::start(float duration, coordStruct pos, float distance, float rampLen) {
  effectSteps = ComputeSteps(duration);  // total number of steps in wipe effect
  deltaRadius = distance / (float) effectSteps;   // compute "speed" of wipe leading edge (mm/step)
  linear = true;
  rampWidth = constrain(rampLen, 1, distance);    // ramp width must be > 0 and <= distance
  center.x = pos.x;
  center.y = pos.y;
  radius = 0;
  completedFlag = false;
  stepNum = 0;
  active = true;
}

void popClass::start(float duration, coordStruct pos, float distance, float rampLen, float accel0, float distFrac0, float accel1) {
  effectSteps = ComputeSteps(duration);  // total number of steps in wipe effect
  rampWidth = constrain(rampLen, 1, distance);    // ramp width must be > 0 and <= distance
  center.x = pos.x;
  center.y = pos.y;
  a0 = accel0;
  a1 = accel1;
  t0 = (uint16_t) round(sqrt((2 * distance * distFrac0) / a0));
  v0 = a0 * (float) t0;
  d0 = (a0 * pow((float) t0, 2)) / 2;
  t1 = (uint16_t) round((float) t0 * 0.5);
  v1 = v0 + (a1 * (float) t1);
  d1 = (v0 * (float) t1) + ((a1 * pow((float) t1, 2)) / 2);
  v2 = (distance - d0 - d1) / (float) (effectSteps - t0 - t1);
  radius = 0;
  completedFlag = false;
  phase = 0;
  phaseStep = 0;
  stepNum = 0;
  linear = false;
  active = true;
  // Serial.printf("Pop: dist=%4.2f t0=%i d0=%4.2f t1=%i d1=%4.2f, v2=%3.2f\n", distance, t0, d0, t1, d1, v2);
}


void popClass::step() {

  if (active) {
    if (linear) {
      radius += deltaRadius;
    }
    else {
      phaseStep++;  // increment here to reflect computations done at END of step period
      switch (phase) {
        case 0:
          radius = (a0 * pow((float) phaseStep, 2)) / 2;
          if (phaseStep >= t0) {
            phase = 1;
            phaseStep = 0;
          }
        break;
        case 1:
          radius = d0 + (v0 * (float) phaseStep) + ((a1 * pow((float) phaseStep, 2)) / 2);
          if (phaseStep >= t1) {
            phase = 2;
            phaseStep = 0;
          }
        break;
        case 2:
          radius += v2;
        break;
        default:
        break;
      }
    }
    stepNum++;
    if (stepNum >= effectSteps) { 
      completedFlag = true;
      active = false;
    }
  }
}


float popClass::value(coordStruct pos) {
  float distFromRadius;

  if (!active)
    return (0.0f);
  distFromRadius = distP2P(pos, center) - radius;
  if (distFromRadius >= 0)   // pop radius hasn't yet crossed the point
    return (0.0f);
  else {  // radius has crossed point, so distance is negative
    distFromRadius = -distFromRadius;   // convert to positive value
    if (distFromRadius > rampWidth)   // back end of ramp has already crossed the point
      return (1.0f);
    else  // point is in between radius and back of ramp
      return (distFromRadius / rampWidth);    // return linear ramp value (0 - 1)
  }
}


float popClass::distP2P(coordStruct p1, coordStruct p2) {
  return (sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2)));
}


bool popClass::completed() {
  bool retVal;

  retVal = completedFlag;
  completedFlag = false;
  return retVal;
}