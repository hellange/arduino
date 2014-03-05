#include <SPI.h>
#include <GD2.h>

class ColortestGD {
 public:
  const static int height = 272;
  const static int width = 480;
  
  ColortestGD();
  byte init();
  void drawRectangle(uint32_t color, int x1,int y1, int x2,int y2);
  void text();
};

