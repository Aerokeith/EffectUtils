#include <Arduino.h>
#include "EffectUtils.h"


#ifndef _FLOW_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _FLOW_TYPES


class flowClass : public effect {
  float deltaDist;  // flow distance per step (mm)
  float rampSlope;  // slope of ramp (delta-value per mm)
  float rampWidth; // distance from flow leading edge to top of ramp (mm)
  bool completedFlag;  // becomes true when flow is completed
public:
  float curPos;   // current position of flow leading edge (mm)
  flowClass() { active = false; completedFlag = false; }
  void start(float duration, float distance, float rampLen);
  void step();
  float val(float offset);
  bool completed();
};

#endif  // _FLOW_TYPES

