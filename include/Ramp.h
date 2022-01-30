#include <Arduino.h>

#ifndef _RAMP_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _RAMP_TYPES

enum phaseEnum {rampUp, hold, rampDown};

class rampClass {
  float rampTime;       // nominal ramp duration as set by setRamp()
  uint16_t rampSteps;   // number of FRAME_PERIOD steps in each up or down ramp (may be reduced based on total duration)
  uint16_t holdSteps;   // number of steps in hold period
  uint16_t stepNum;     // current step number
  phaseEnum phase;      // current ramp phase  
  float rampDelta;      // added to val each ramp step
public:
  bool active;    // ramp function is active (any ramp phase)
  float val;      // current output value of ramp function (0 - 1)
  rampClass();
  void start(float duration);
  void start(float duration, float rampDur);
  void setRamp(float rampTime);
  void step();
};

#endif  // _RAMP_TYPES
