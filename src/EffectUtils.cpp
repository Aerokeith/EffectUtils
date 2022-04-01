#include <Arduino.h>
#include "EffectUtils.h"

  // static member of effect class, declared here and initialized to default that can be overridden with call to
  //  effect::SetStepPeriod()
float effect::stepPeriod = defaultStepPeriod;


/* effect::ComputeSteps()
    Computes the number of effect steps (based on stepPeriod) in a specified duration
    Parameters:
      float duration: 
    Returns:
      uint16_t: Number of stepPeriod steps in duration. Returns 0 if (duration == 0); otherwise return value is >= 1
*/
uint16_t effect::ComputeSteps(float duration) { 
  if (duration == 0) 
    return 0; 
  else
    return (uint16_t) ceil(duration / stepPeriod); 
} 

