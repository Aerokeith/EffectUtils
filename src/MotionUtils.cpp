#include <Arduino.h>
#include "MotionUtils.h"

  // static member of motion class, declared here and initialized to default that can be overridden with call to
  //  motion::SetStepPeriod()
float motion::stepPeriod = defaultStepPeriod;

