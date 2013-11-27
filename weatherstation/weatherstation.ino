// Weather Station with 32' TFT LCD 240x400 Display.
// LCD driver driver based on:
// UTFT_Demo_400x240 (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics

// BMP085 Sensor driver based on code found on Adafruit web pages.

// Author:
// Helge Langehaug

//#include <weatherstation.h>
#include <UTFT.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

//
// Declare which fonts we will be using
extern uint8_t SmallFont[];


// Using Arduino Mega and 3.2' TFT LCD 240x400 Display
UTFT myGLCD(TFT01_32WD,38,39,40,41);
int counter = 0;

Adafruit_BMP085 bmp;


// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// 
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Wire.h>
#define DS1307_ADDRESS 0x68

/*
/* Downloaded from http://projectsfromtech.blogspot.com/
*Connect SCL, SDA, Vcc, and GND
*Open Serial Monitor and enjoy!
*/
/*
//Arduino 1.0+ Only

#include "Wire.h"



void setupx3(){
  Wire.begin();
  Serial.begin(9600);
}

void loopx3(){
  printDate();
  delay(1000);
}
*/
byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

void printDate(){

  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());

  //print the date EG   3/1/11 23:59:59
  Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

}
 


void setup()
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
  
  setupDisplay();
}


void loop()
{
  byte error, address;
  int nDevices;
  char buffer[4];
  int scanTimes = 0;
     
  myGLCD.clrScr();
  Serial.println("Scanning...");
  myGLCD.print("Scanning...", CENTER, 10);
  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      myGLCD.print("I2C device found at address", CENTER, 30+nDevices*20);
 
      sprintf(buffer, "%02X", address);
      
            myGLCD.print(buffer, CENTER, 40+nDevices*20);

      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      myGLCD.print("No I2C devices found", CENTER, 30);

      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0){
    Serial.println("No I2C devices found\n");
    myGLCD.print("No I2C devices found", CENTER, 30);
  }
  else{
    Serial.println("done\n");
    myGLCD.print("done", CENTER, 20);
  }
  
  
  
  if (!bmp.begin()) {
    myGLCD.print("Could not find a valid BMP085 pressure sensor.", CENTER, 130);
    myGLCD.print("Anything connected at all ???", CENTER, 145);

      
  } else {
    myGLCD.print("Preasure", CENTER, 120);
    myGLCD.printNumF(bmp.readPressure()/100, 2, CENTER, 130);
  }

  printDate();

  delay(4000);           // wait 5 seconds for next scan
}



void setupDisplay()
{
  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();
  //myGLCD.setColor(0, 255, 0);
  //#myGLCD.drawCircle(100,120,60);
}

/*

void setupx2()
{
  randomSeed(analogRead(0));

// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);


  myGLCD.clrScr();


  

  myGLCD.setColor(255, 255, 255);

  myGLCD.print("Weather Station V0.3", CENTER, 80);

  myGLCD.setColor(255, 0, 0);
  if (!bmp.begin()) {
      myGLCD.print("Could not find a valid BMP085 pressure sensor.", CENTER, 130);
      myGLCD.print("Anything connected at all ???", CENTER, 145);

      while (1) {}
  }


}

void loopx2()
{
  counter = counter + 1;
  myGLCD.setColor(255, 255, 255);
  myGLCD.printNumF(bmp.readPressure()/100, 2, CENTER, 120);
  delay(1000);


}

*/

