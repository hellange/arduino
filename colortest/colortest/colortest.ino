#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

void setup()
{

  GD.begin();
  GD.ClearColorRGB(0x104000);
  GD.Clear();
}

int x = 0;
int width = 48;
int height = 140;
const int nr = 10;
const uint32_t row1[nr]  = {0xff0000, 0x00ff00, 0x0000ff, 0x880000, 0x008800, 0x000088, 0x888888, 0xffff00, 0x00ffff, 0xff00ff}; 
const uint32_t row2[nr] = {0x0060c0, 0xc060e0, 0x003333, 0x009999, 0x104000, 0x001040, 0x004080, 0x404040, 0x440000, 0x004400};

void loop(){
  GD.ClearColorRGB(0x104000);
  GD.Clear();

  // first row
  for (int i = 0; i<nr; i++){ 
    GD.ColorRGB(row1[i]);
    GD.Begin(RECTS);
    GD.Vertex2ii(i*width , 0 );
    GD.Vertex2ii((i+1)*width, height );
  }
  
  // second row
  for (int i = 0; i<nr; i++){ 
    GD.ColorRGB(row2[i]);
    GD.Begin(RECTS);
    GD.Vertex2ii(i*width , height );
    GD.Vertex2ii((i+1)*width, height*2  );
  } 
    
  GD.ColorRGB(0x000000);
  GD.cmd_text(240, height / 2, 31, OPT_CENTER, "T e s t i n g   c o l o r s");
  GD.ColorRGB(0xffffff);
  GD.cmd_text(240, height + height/2, 31, OPT_CENTER, "T e s t i n g   c o l o r s");

  GD.swap();

  while (1==1);

  
  GD.swap();

}
