#include <Arduino.h>
#include "Lines.h"


void movingLineClass::init(coordStruct refPos, float angle) {
  float angleRad;

  point = refPos;
  angleRad = (angle/360.0) * TWO_PI;
  coeffA = -sin(angleRad);  // compute coefficients of line equation Ax + By + C = 0
  coeffB = cos(angleRad);
  coeffC = -(coeffA * point.x) - (coeffB * point.y);
  trajDelta.x = coeffA;   // compute (∆x, ∆y) of reference point position for each mm of travel along trajectory  (pependicular to line) 
  trajDelta.y = coeffB;
}


void movingLineClass::step(float distance) {
  point.x += trajDelta.x * distance;
  point.y += trajDelta.y * distance;
  coeffC = -(coeffA * point.x) - (coeffB * point.y);
}


float movingLineClass::distance(coordStruct pos) {
  return ((coeffA * pos.x) + (coeffB * pos.y) + coeffC);
}
