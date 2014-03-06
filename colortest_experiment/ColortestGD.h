#include <SPI.h>
#include <GD2.h>
#include "Colortest.h"

#ifndef ColortestGD_h
#define ColortestGD_h

class ColortestGD: public Colortest {
 public:
  ColortestGD();
  int width;
  int height;
  byte init();
  void drawRectangle(uint32_t color, int x1,int y1, int x2,int y2);
  void text();
};

#endif
