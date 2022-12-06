#include <Arduino.h>

#ifndef _STRIPCOORDS_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _STRIPCOORDS_TYPES

struct stripCoordStruct {
  float x;
  float y;
};
struct stripDefStruct {
  stripCoordStruct position;
  float angle;
  float pixelSpacing;
};

class stripCoordClass {
  stripCoordStruct origin;
  float theta;
  float spacing;
  stripCoordStruct coord;
  stripCoordStruct delta;
public:
  void define(const stripDefStruct *stripDef);
  stripCoordStruct getCoord(uint16_t pixel);
  stripCoordStruct getCoord();
  float getPos(uint16_t pixel);
  float getPos();
};


#endif  // _STRIPCOORDS_TYPES
