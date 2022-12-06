#include <Arduino.h>
#include "StripMgr.h"

  void stripMgrClass::define(uint16_t pixels, uint8_t segments, float spacing, const segmentDefStruct *firstSegment) {
    totalPixels = pixels;
    numSegments = segments;
    pixelSpacing = spacing;
    segPtr = firstSegment;
    endOfStrip = true;
  }

  void stripMgrClass::initSegment(uint16_t segPix) {
    segPixel = segPix;
    theta = ((segPtr + segment)->angle / 360.0) * TWO_PI;
    delta.x = cos(theta) * pixelSpacing;
    delta.y = sin(theta) * pixelSpacing;
    coord.x = (segPtr + segment)->position.x + (delta.x * segPixel);
    coord.y = (segPtr + segment)->position.y + (delta.y * segPixel);
}


  stripCoordStruct stripMgrClass::getCoord(uint16_t pixel) {
    uint16_t pixCount;
    int excessPixels;

    if (pixel >= (totalPixels)) {
      endOfStrip = true;
      return {0, 0};
    }
    else {
      endOfStrip = false;
      segment = 0;
      pixCount = segPtr->numPixels;
      excessPixels = pixCount - (pixel + 1);
      while ((excessPixels < 0) && (segment < (numSegments - 1))) {
        segment++;
        pixCount += (segPtr + segment)->numPixels;
        excessPixels = pixCount - (pixel + 1);
      }
      if (excessPixels < 0) {
        endOfStrip = true;
        return {0, 0};
      }
      initSegment((segPtr+segment)->numPixels - excessPixels - 1);
      return coord;
    }
  }

  stripCoordStruct stripMgrClass::getCoord() {
    stripCoordStruct retCoord;

    retCoord = coord;
    if (segPixel == ((segPtr+segment)->numPixels - 1)) {  // if last pixel in segment
      if (segment >= (numSegments - 1)) {   // and if this is the last segment
        endOfStrip = true;
      }
      else {  // not the last segment
        segment++;
        initSegment(0);
      }
    }
    else {  // not the last pixel in the current segment
      coord.x += delta.x;
      coord.y += delta.y;
      segPixel++;
    }
    return retCoord;
  }

