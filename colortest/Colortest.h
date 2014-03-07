#include <SPI.h>

#ifndef Colortest_h
#define Colortest_h

class Colortest {
 public:
  int width;
  int height;
  virtual byte init() = 0;
  virtual void drawRectangle(uint32_t color, int x1,int y1, int x2,int y2) = 0;
  virtual void text() = 0;
};

#endif

