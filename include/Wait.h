#include <Arduino.h>
#include "EffectUtils.h"

#ifndef _WAIT_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _WAIT_TYPES

class waitClass : public effect {    // derived from "effect" class defined in EffectUtils.h
  bool completedFlag;  // becomes true when flow is completed
public:
  waitClass() { active = false; completedFlag = false;}   // object constructor
  void start(float duration);
  void step();
  bool completed();
};

#endif  // _WAIT_TYPES

