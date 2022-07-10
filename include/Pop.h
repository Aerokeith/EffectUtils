#include <Arduino.h>
#include "EffectUtils.h"
#include "Lines.h"


#ifndef _POP_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _POP_TYPES


class popClass : public effect {
  coordStruct center;
  float radius;
  float deltaRadius;  // amount to increase pop radius per step (mm)
  uint8_t phase;
  uint16_t phaseStep; 
  uint16_t t0, t1;
  float a0, v0, d0;
  float a1, v1, d1;
  float v2;
  float rampWidth; // distance from pop radius to top of ramp (mm)
  bool linear;
  bool completedFlag;  // becomes true when pop is completed
public:
  popClass() { active = false; completedFlag = false; }
  void start(float duration, coordStruct pos, float distance, float rampLen);
  void start(float duration, coordStruct pos, float distance, float ramplen, float accel0, float distFrac0, float accel1);
  void step();
  float value(coordStruct pos);
  float distP2P(coordStruct p1, coordStruct p2);
  bool completed();
};

#endif  // _POP_TYPES

