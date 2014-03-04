#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

UTFT myGLCD(CTE35IPS,38,39,40,41);   // 320x480
int width = 30;
int height = 100;

//UTFT myGLCD(TFT01_32WD,38,39,40,41);   // 240x400
//int width = 30;
//int height = 100;

void setup()
{
}

int x = 0;

const int nr = 10;
const uint32_t row1[nr] = {0xff0000, 0x00ff00, 0x0000ff, 0x880000, 0x008800, 0x000088, 0x888888, 0xffff00, 0x00ffff, 0xff00ff}; 
const uint32_t row2[nr] = {0x0060c0, 0xc060e0, 0x003333, 0x009999, 0x104000, 0x001040, 0x004080, 0x404040, 0x440000, 0x004400};

void drawGDRectangle(uint32_t color, int x1,int y1, int x2,int y2){
 
 unsigned char r = *((unsigned char *)&color);  
 unsigned char g = *((unsigned char *)&color +1);  
 unsigned char b = *((unsigned char *)&color +2);  
 
 myGLCD.setColor(r,g,b);
 myGLCD.fillRect(x1, y1, x2, y2);
}

void drawGD(){

  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  myGLCD.fillScr(0,0,0);

  // first row
  for (int i = 0; i<nr; i++){ 
    drawGDRectangle(row1[i], i*width, 0, (i+1)*width, height);
  }
  
  // second row
  for (int i = 0; i<nr; i++){ 
    drawGDRectangle(row2[i], i*width, height, (i+1)*width, height*2);
  } 
    
   myGLCD.setColor(00,00,00);

  //GD.cmd_text(240, height / 2, 31, OPT_CENTER, "T e s t i n g   c o l o r s");
   myGLCD.print("T e s t i n g   c o l o r s", 20, height / 2);
   
   myGLCD.setColor(255,255,255);
   
  //GD.cmd_text(240, height + height/2, 31, OPT_CENTER, "T e s t i n g   c o l o r s");
     myGLCD.print("T e s t i n g   c o l o r s", 20, height + height / 2);

}

void loop(){
  drawGD();
  while (1==1);

}
