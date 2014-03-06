#include <SPI.h>

class ColortestUTFT {
 public:
  ColortestUTFT();
  int height;
  int width;
  byte init();
  void drawRectangle(uint32_t color, int x1,int y1, int x2,int y2);
  void text();
};

