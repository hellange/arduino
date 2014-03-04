#include <UTFT.h>

extern uint8_t BigFont[];

const int bars = 10;
UTFT myGLCD(CTE35IPS,38,39,40,41);   // 320x480
int width = 480/bars;
int height = 320/2 -1 ;

//UTFT myGLCD(TFT01_32WD,38,39,40,41);   // 240x400
//int width = 400/10;
//int height = 240/2;

void setup()
{
}

int x = 0;

const uint32_t row1[bars] = {0xff0000, 0x00ff00, 0x0000ff, 0x880000, 0x008800, 0x000088, 0x888888, 0xffff00, 0x00ffff, 0xff00ff}; 
const uint32_t row2[bars] = {0x0060c0, 0xc060e0, 0x003333, 0x009999, 0x104000, 0x001040, 0x004080, 0x404040, 0x440000, 0x004400};

void drawGDRectangle(uint32_t color, int x1,int y1, int x2,int y2){
 
 unsigned char r = *((unsigned char *)&color);  
 unsigned char g = *((unsigned char *)&color +1);  
 unsigned char b = *((unsigned char *)&color +2);  
 
 myGLCD.setColor(r,g,b);
 myGLCD.fillRect(x1, y1, x2, y2);
}

void drawGD(){

  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);
  myGLCD.fillScr(0,0,0);

  // first row
  for (int i = 0; i<bars; i++){ 
    drawGDRectangle(row1[i], i*width, 0, (i+1)*width, height);
  }
  
  // second row
  for (int i = 0; i<bars; i++){ 
    drawGDRectangle(row2[i], i*width, height, (i+1)*width, height*2);
  } 
   
  myGLCD.setBackColor(255,255,255); 
  myGLCD.setColor(0,0,0);
  myGLCD.print("T e s t i n g   c o l o r s", 20, height / 2);
   
  myGLCD.setBackColor(0,0,0); 
  myGLCD.setColor(255,255,255);
  myGLCD.print("T e s t i n g   c o l o r s", 20, height + height / 2);

}

void loop(){
  drawGD();
  while (1==1);

}
