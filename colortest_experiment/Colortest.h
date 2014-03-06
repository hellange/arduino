#include <SPI.h>

#ifndef Colortest_h
#define Colortest_h

class Colortest {
 public:
  Colortest();
  int width;
  int height;
  virtual byte init();
  void drawRectangle(uint32_t color, int x1,int y1, int x2,int y2);
  void text();
};

#endif

