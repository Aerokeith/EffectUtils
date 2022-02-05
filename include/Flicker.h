#include <Arduino.h>
#include "MotionUtils.h"
#include "Ramp.h"

#ifndef _FLICK_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _FLICK_TYPES

class flickerClass : motion {   // derived from "motion" class
  uint16_t cycleSteps;    // number of steps in each flicker cycle
  uint16_t cycleStepNum;  // step number in a cycle
  rampClass ramp;       // embedded ramp function
  float flickVal;     // flicker function value, not scaled by ramp
  float maxDelta;       // max change in flickVal per step
  float targetVal;     // current (random) value being applied in this cycle
public:
  flickerClass() {active = false;};
  void start(float duration, float frequency, float filter);
  void step();
  void setRamp(float rampTime);
  float val();
};

#endif  // _FLICK_TYPES
