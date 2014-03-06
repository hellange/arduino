#include "ColortestUTFT.h"
#include <UTFT.h>

extern uint8_t BigFont[];

UTFT myGLCD(CTE35IPS,38,39,40,41);   // 320x480
//UTFT myGLCD(TFT01_32WD,38,39,40,41);   // 240x400

ColortestUTFT::ColortestUTFT(){
  
  //CTE35IPS
  width = 480 - 10;
  height = 320 - 2;
  
  //TFT01_32WD
  //width = 400;
  //height = 240;
}

byte ColortestUTFT::init(){
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);
  myGLCD.fillScr(0,0,0);
}

void ColortestUTFT::drawRectangle(uint32_t color, int x1,int y1, int x2,int y2){
 unsigned char r = *((unsigned char *)&color +2);  
 unsigned char g = *((unsigned char *)&color +1);  
 unsigned char b = *((unsigned char *)&color);  
 
 myGLCD.setColor(r,g,b);
 myGLCD.fillRect(x1, y1, x2, y2);

}

void ColortestUTFT::text(){
  myGLCD.setBackColor(255,255,255); 
  myGLCD.setColor(0,0,0);
  myGLCD.print("T e s t i n g   c o l o r s", 20, height / 4);
   
  myGLCD.setBackColor(0,0,0); 
  myGLCD.setColor(255,255,255);
  myGLCD.print("T e s t i n g   c o l o r s", 20, 3 * height / 4);

}
