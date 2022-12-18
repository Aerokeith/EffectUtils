#include <Arduino.h>
#include "EffectUtils.h"
#include "Ramp.h"

#ifndef _FLICK_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _FLICK_TYPES

class flickerClass : public effect {   // derived from "effect" class defined in EffectUtils.h
  uint16_t cycleSteps;    // number of steps in each flicker cycle
  uint16_t cycleStepNum;  // step number in a cycle
  rampClass ramp;       // embedded ramp function
  float flickVal;     // flicker function value, not scaled by ramp
  uint32_t minTarget;   // minimum target for flickVal (integer format) based on start() parameter
  float maxDelta;       // max change in flickVal per step
  float targetVal;     // current (random) value being applied in this cycle
public:
  flickerClass() {active = false;};
  void start(float duration, float frequency, float filter, float minVal);
  void step();
  void setRamp(float rampTime);
  void setRamp(float rampUpTime, float rampDownTime);
  float val();
};

#endif  // _FLICK_TYPES
