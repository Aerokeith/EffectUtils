#include <Arduino.h>
#include "EffectUtils.h"

#ifndef _RAMP_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _RAMP_TYPES

enum phaseEnum {rampUp, hold, rampDown};

class rampClass : public effect {    // derived from base class "effect" defined in EffectUtils.h
  float rampUpTime;       // nominal ramp up duration as set by start() or setRamp()
  float rampDownTime;     // nominal ramp down duration as set by start() or setRamp()
  uint16_t rampUpSteps;  // number of FRAME_PERIOD steps in up ramp phase (may be reduced based on total duration)
  uint16_t rampDownSteps;  // number of steps in down ramp phase (may be reduced based on total duration)
  uint16_t holdSteps;   // number of steps in hold period
  phaseEnum phase;      // current ramp phase  
  float rampDelta;      // added to val each ramp step
public:
  float val;      // current output value of ramp function (0 - 1)
  rampClass() { rampUpTime = 0; rampDownTime = 0; active = false; }
  void start(float duration);
  void start(float duration, float rampDur);
  void start(float duration, float rampUpDur, float rampDownDur);
  void setRamp(float rampDur);
  void setRamp(float rampUpDur, float rampDownDur);
  void step();
};

#endif  // _RAMP_TYPES
