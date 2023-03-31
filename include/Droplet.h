#include <Arduino.h>
#include "EffectUtils.h"


#ifndef _DROPLET_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _DROPLET_TYPES

struct dropletConfigStruct {
  float minTailLength;      // minimum droplet tail length (mm)
  float maxTailLength;      // maximum overall length (mm)
  float headLength;         // length of droplet head (mm)
  float initVelocity;       // initial droplet velocity (mm/sec)
  float acceleration;        // droplet acceleration (mm/sec^2)
  float headRampLen;        // length of leading edge (head) ramp (mm)
};

class dropletClass : public effect {
  float distance;     // total distance to be traversed by droplet leading edge
  float tailLength;   // length of current (randomized) droplet tail
  float deltaDist;    // flow distance per step (mm)
  float accelDelta;   // acceleration: increase in deltaDist per step
  float headSlope;    // slope of head ramp (delta-value per mm)
  float tailSlope;    // slope of tail ramp (delta-value per mm)
  bool completedFlag;  // becomes true when flow is completed
  dropletConfigStruct *config;  // pointer to structure containing configuration parameters
public:
  float curPos;   // current position of flow leading edge (mm)
  dropletClass() { active = false; completedFlag = false; }
  void init(dropletConfigStruct *cfg) { config = cfg; }
  void start(float dist);
  void step();
  float value(float offset);
  bool completed();
};

#endif  // _DROPLET_TYPES

