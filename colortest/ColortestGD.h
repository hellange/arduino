#include <SPI.h>
#include <GD2.h>
#include "Colortest.h"

class ColortestGD : public Colortest{
 public:
  ColortestGD();

  const static int height = 272;
  const static int width = 480;
  
  byte init();
  void drawRectangle(uint32_t color, int x1,int y1, int x2,int y2);
  void text();
};

