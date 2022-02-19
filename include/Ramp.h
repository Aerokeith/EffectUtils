#include <Arduino.h>
#include "EffectUtils.h"

#ifndef _RAMP_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _RAMP_TYPES

enum phaseEnum {rampUp, hold, rampDown};

class rampClass : public effect {    // derived from base class "effect" defined in EffectUtils.h
  float rampTime;       // nominal ramp duration as set by start() or setRamp()
  uint16_t rampSteps;   // number of FRAME_PERIOD steps in each up or down ramp (may be reduced based on total duration)
  uint16_t holdSteps;   // number of steps in hold period
  phaseEnum phase;      // current ramp phase  
  float rampDelta;      // added to val each ramp step
public:
  float val;      // current output value of ramp function (0 - 1)
  rampClass() { rampTime = 0; active = false; }
  void start(float duration);
  void start(float duration, float rampDur);
  void setRamp(float rampDur);
  void step();
};

#endif  // _RAMP_TYPES
