#include <Arduino.h>

#ifndef _EFFECT_UTIL_TYPES
#define _EFFECT_UTIL_TYPES

const float defaultStepPeriod = 0.01;   // default, overridden by call to SetStepPeriod()

/*
  A "core" class for the entire EffectUtils library, containing common data members and functions to be used by all derived classes
*/
class effect {
public:
  static float stepPeriod;    // duration of each effect step
  bool active;                // true during the specified duration of the effect
  uint16_t stepNum;           // current step number
  uint16_t effectSteps;       // total number of steps in the specified effect duration
    // Compute the number of effect steps in the specified duration
  uint16_t ComputeSteps(float duration); 
    // Set the step period, which will be propagated to all derived classes/objects
  static void SetStepPeriod(uint32_t periodMs) { stepPeriod = (float) periodMs / 1000; }
};

#endif  // _EFFECT_UTIL_TYPES