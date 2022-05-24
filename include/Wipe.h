#include <Arduino.h>
#include "EffectUtils.h"
#include "Lines.h"


#ifndef _WIPE_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _WIPE_TYPES


class wipeClass : public effect {
  movingLineClass line;   // moving line representing the leading edge of the wipe effect
  float deltaDist;  // distance to move wipe line per step (mm), in direction perpendicular to line
  float rampWidth; // distance from flow leading edge to top of ramp (mm)
  bool completedFlag;  // becomes true when flow is completed
public:
  wipeClass() { active = false; completedFlag = false; }
  void start(float duration, coordStruct refPos, float angle, float distance, float rampLen);
  void step();
  float value(coordStruct pos);
  bool completed();
};

#endif  // _WIPE_TYPES

