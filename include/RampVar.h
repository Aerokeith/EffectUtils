#include <Arduino.h>
#include "EffectUtils.h"

#ifndef _RAMPVAR_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _RAMPVAR_TYPES

class rampVarClass : public effect {    // derived from base class "effect" defined in EffectUtils.h
  float *varPtr;        // pointer to the variable being ramped
  uint16_t rampSteps;   // number of FRAME_PERIOD steps in the ramp duration
  float rampDelta;      // added to *varPtr each ramp step
  float endVal;         // copy of the targetVal parameter, used to eliminate any rounding effects at the end of the ramp
public:
  rampVarClass() { active = false; }
  void start(float *var, float targetVal, float rampDur);
  void step();
};

#endif  // _RAMPVAR_TYPES
