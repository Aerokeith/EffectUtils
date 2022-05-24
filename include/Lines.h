#include <Arduino.h>

#ifndef _LINE_TYPES
#define _LINE_TYPES

struct coordStruct {
  float x;
  float y;
};


class movingLineClass {
  coordStruct point;  // reference point for line
  float coeffA, coeffB, coeffC; // coefficients of line equation Ax + By + C = 0
  coordStruct trajDelta;  // (∆x, ∆y) of reference point position for each mm of travel along trajectory  (pependicular to line)
public:
  void init(coordStruct refPos, float angle);
  void step(float distance);
  float distance(coordStruct pos);
};


#endif    // _LINE_TYPES