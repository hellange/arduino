#include <EEPROM.h>
#include <SPI.h>
#include "ColortestGD.h"
//#include "ColortestUTFT.h"
#include "Colortest8875.h"
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
#include <UTFT.h>
#include <GD2.h>

//ColortestGD ct = ColortestGD();
//ColortestUTFT ct = ColortestUTFT();
Colortest8875 ct = Colortest8875();

void setup()
{
  Serial.begin(9600);
  Serial.println("Init");
  ct.init();
  Serial.println("Initialized");
}

// RGB888 based color bars
const int bars = 10;
const uint32_t row1[bars] = {0xff0000, 0x00ff00, 0x0000ff, 0x880000, 0x008800, 0x000088, 0x888888, 0xffff00, 0x00ffff, 0xff00ff}; 
const uint32_t row2[bars] = {0x0060c0, 0xc060e0, 0x003333, 0x009999, 0x104000, 0x001040, 0x004080, 0x404040, 0x440000, 0x004400};

void drawIt(){

  int barWidth = ct.width/bars;
  // first row
  for (int i = 0; i<bars; i++){ 
    ct.drawRectangle(row1[i], i*barWidth, 0, (i+1)*barWidth, ct.height/2);
  }
  
  // second row
  for (int i = 0; i<bars; i++){ 
    ct.drawRectangle(row2[i], i*barWidth, ct.height/2, (i+1)*barWidth, ct.height);
  } 
    
  ct.text();
}

void loop(){
  drawIt();
  while (1==1);

}
