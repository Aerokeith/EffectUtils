#include <Arduino.h>

#ifndef _STRIPMGR_TYPES  // prevent duplicate type definitions when this file is included in multiple places
#define _STRIPMGR_TYPES

const uint8_t maxSegments = 8;

struct stripCoordStruct {
  float x;
  float y;
};

struct segmentDefStruct {
  stripCoordStruct position;
  float angle;
  uint16_t numPixels;
};

class stripMgrClass {
  uint8_t numSegments;
  uint16_t totalPixels;
  float pixelSpacing;
  uint16_t segPixel;;
  const segmentDefStruct *segPtr;
  uint8_t segment;
  float theta;
  stripCoordStruct coord;
  stripCoordStruct delta;
  bool endOfStrip;
  void initSegment(uint16_t segPix);
public:
  void define(uint16_t pixels, uint8_t segments, float spacing, const segmentDefStruct *firstSegment);
  stripCoordStruct getCoord(uint16_t pixel);
  stripCoordStruct getCoord();
};

#endif  // _STRIPMGR_TYPES
