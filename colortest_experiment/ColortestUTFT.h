#include <SPI.h>
#include "Colortest.h"

#ifndef ColortestUTFT_h
#define ColortestUTFT_h

class ColortestUTFT: public Colortest {
 public:
  ColortestUTFT();
  int width;
  int height;
  byte init();
  void drawRectangle(uint32_t color, int x1,int y1, int x2,int y2);
  void text();
};

#endif
