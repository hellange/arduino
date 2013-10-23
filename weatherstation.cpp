// Weather Station with 32' TFT LCD 240x400 Display.
// LCD driver driver based on:
// UTFT_Demo_400x240 (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics

// BMP085 Sensor driver based on code found on Adafruit web pages.

// Author:
// Helge Langehaug

#include "weatherstation.h"

//
// Declare which fonts we will be using
extern uint8_t SmallFont[];


// Using Arduino Mega and 3.2' TFT LCD 240x400 Display
UTFT myGLCD(TFT01_32WD,38,39,40,41);
int counter = 0;

Adafruit_BMP085 bmp;

void setup()
{
  randomSeed(analogRead(0));

// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);


  myGLCD.clrScr();


  myGLCD.setColor(0, 255, 0);
  myGLCD.drawCircle(100,120,60);

  myGLCD.setColor(255, 255, 255);

  myGLCD.print("Weather Station V0.3", CENTER, 80);

  myGLCD.setColor(255, 0, 0);
  if (!bmp.begin()) {
      myGLCD.print("Could not find a valid BMP085 pressure sensor.", CENTER, 130);
      myGLCD.print("Anything connected at all ???", CENTER, 145);

      while (1) {}
  }


}

void loop()
{
  counter = counter + 1;
  myGLCD.setColor(255, 255, 255);
  myGLCD.printNumF(bmp.readPressure()/100, 2, CENTER, 120);
  delay(1000);


}



