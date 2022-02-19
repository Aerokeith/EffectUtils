#include <Arduino.h>
#include "EffectUtils.h"

  // static member of effect class, declared here and initialized to default that can be overridden with call to
  //  effect::SetStepPeriod()
float effect::stepPeriod = defaultStepPeriod;

