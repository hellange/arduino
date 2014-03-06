#include "ColortestGD.h"
#include <GD2.h>

ColortestGD::ColortestGD(){
  width = 480;
  height = 272;
}

byte ColortestGD::init(){
  GD.begin();
  GD.ClearColorRGB(0x104000);
  GD.Clear();
}

void ColortestGD::drawRectangle(uint32_t color, int x1,int y1, int x2,int y2){
  GD.ColorRGB(color);
  GD.Begin(RECTS);
  GD.Vertex2ii(x1, y1);
  GD.Vertex2ii(x2, y2);
}

void ColortestGD::text(){
  GD.ColorRGB(0x000000);
  GD.cmd_text(240, height / 4, 31, OPT_CENTER, "T e s t i n g   c o l o r s");
  GD.ColorRGB(0xffffff);
  GD.cmd_text(240, 3 * height / 4, 31, OPT_CENTER, "T e s t i n g   c o l o r s");
  GD.swap();
}
