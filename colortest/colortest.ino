// Weather Station with LCD TFT display
// Uses several libraries, ref link at the #include lines
//
// Author:
// Helge Langehaug

#include <UTFT.h>            // http://www.henningkarlsen.com/electronics
#include <UTouch.h>          // http://www.henningkarlsen.com/electronics

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

// Using Arduino Mega and 3.2' TFT LCD 240x400 Display
UTFT myGLCD(CTE35IPS,38,39,40,41);
UTouch myTouch(6,5,4,3,2);


void setup(){
  Serial.begin(9600);
  
 myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  myGLCD.fillScr(0,0,0);
  //myGLCD.setBackColor(240,240,240);
  myGLCD.setColor(0, 255, 0);


  
}


void loop(){
  myGLCD.setBackColor(0,0,0);

 // draw it
  myGLCD.setColor(10,50,10);
  myGLCD.fillRect(10,10,50,50);
  
 
}



 




