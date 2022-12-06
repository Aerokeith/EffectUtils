#include <Arduino.h>
#include "StripCoord.h"

  void stripCoordClass::define(const stripDefStruct *stripDef) {
    origin = stripDef->position;
    theta = (stripDef->angle / 360.0) * TWO_PI;
    spacing = stripDef->pixelSpacing;
    delta.x = cos(theta) * spacing;
    delta.y = sin(theta) * spacing;
  }

  stripCoordStruct stripCoordClass::getCoord(uint16_t pixel) {
    coord.x = origin.x + (delta.x * pixel);
    coord.y = origin.y + (delta.y * pixel);
    return coord;
  }

  stripCoordStruct stripCoordClass::getCoord() {
    stripCoordStruct retVal;
    retVal = coord;
    coord.x += delta.x;
    coord.y += delta.y;
    return retVal;
  }

  float stripCoordClass::getPos(uint16_t pixel) {
    coord.x = origin.x + (delta.x * pixel);
    return coord.x;
  }

  float stripCoordClass::getPos()  {
    float retVal;
    retVal = coord.x;
    coord.x += delta.x;
    return retVal;
  }

