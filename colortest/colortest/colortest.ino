#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

void setup()
{
}

int x = 0;
const int bars = 10;

int width = 480 / bars;
int height = 272 / 2;
const uint32_t row1[bars] = {0xff0000, 0x00ff00, 0x0000ff, 0x880000, 0x008800, 0x000088, 0x888888, 0xffff00, 0x00ffff, 0xff00ff}; 
const uint32_t row2[bars] = {0x0060c0, 0xc060e0, 0x003333, 0x009999, 0x104000, 0x001040, 0x004080, 0x404040, 0x440000, 0x004400};

void drawGDRectangle(uint32_t color, int x1,int y1, int x2,int y2){
    GD.ColorRGB(color);
    GD.Begin(RECTS);
    GD.Vertex2ii(x1 , y1 );
    GD.Vertex2ii(x2, y2 );
}

void drawGD(){
  GD.begin();
  GD.ClearColorRGB(0x104000);
  GD.Clear();

  // first row
  for (int i = 0; i<bars; i++){ 
    drawGDRectangle(row1[i], i*width, 0, (i+1)*width, height);
  }
  
  // second row
  for (int i = 0; i<bars; i++){ 
    drawGDRectangle(row2[i], i*width, height, (i+1)*width, height*2);
  } 
    
  GD.ColorRGB(0x000000);
  GD.cmd_text(240, height / 2, 31, OPT_CENTER, "T e s t i n g   c o l o r s");
  GD.ColorRGB(0xffffff);
  GD.cmd_text(240, height + height/2, 31, OPT_CENTER, "T e s t i n g   c o l o r s");
  GD.swap();
}

void loop(){
  drawGD();
  while (1==1);

}
