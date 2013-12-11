// Weather Station with 32' TFT LCD 240x400 Display.
// LCD driver driver based on:
// UTFT_Demo_400x240 (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics

// BMP085 Sensor driver based on code found on Adafruit web pages.

// Author:
// Helge Langehaug

//#include <weatherstation.h>
#include <UTFT.h>
#include <UTouch.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

//
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

// Using Arduino Mega and 3.2' TFT LCD 240x400 Display
UTFT myGLCD(TFT01_32WD,38,39,40,41);
UTouch      myTouch(6,5,4,3,2);
int counter = 0;
int x, y;
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

/* Downloaded from http://projectsfromtech.blogspot.com/
*Connect SCL, SDA, Vcc, and GND
*Open Serial Monitor and enjoy!
*/

//Arduino 1.0+ Only

#include "Wire.h"
#define DS1307_ADDRESS 0x68
  byte zero = 0x00;

void setup(){
  Wire.begin();
  Serial.begin(9600);
  randomSeed(analogRead(0)); // ??
  //setDateTime();
  printDate();
  initGraphics();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  myGLCD.print("I2C DEVICES !!!", 20, 80,315);
  
  
  printDate();
  showPressure();
  
  scan();
}

void loop(){
  printDate();
  showPressure();
  
  for (int i = 0; i<18; i++){
    checkTouch();
    delay(50);
  }
}

void checkTouch(){
  if (myTouch.dataAvailable()) {
    myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
      myGLCD.printNumI(x, 0, 0, 3);
      myGLCD.printNumI(y, 50, 0, 3);
      myGLCD.fillCircle(x,y,10);
  }
}

byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}
byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}
void setDateTime(){

  byte second =      30; //0-59
  byte minute =      35; //0-59
  byte hour =        19; //0-23
  byte weekDay =     3; //1-7
  byte monthDay =    4; //1-31
  byte month =       12; //1-12
  byte year  =       13; //0-99

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //stop Oscillator

  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));

  Wire.write(zero); //start 

  Wire.endTransmission();

}
void printDate(){

  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);

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
    myGLCD.setColor(0, 255, 0);

  myGLCD.setFont(BigFont);
    myGLCD.printNumI(monthDay, 20, 110, 2);
    
        myGLCD.print("/", 50, 110);

    myGLCD.printNumI(month, 70, 110, 2);

        myGLCD.print("/", 100, 110);

    myGLCD.printNumI(year, 120, 110, 2);

    myGLCD.printNumI(hour, 190, 110, 2 ,'0');
        myGLCD.print(":", 225, 110);

    myGLCD.printNumI(minute, 245, 110, 2 ,'0');
    myGLCD.print(":", 280, 110);
    myGLCD.printNumI(second, 300, 110, 2 ,'0');
}





void scan()
{
    myGLCD.setColor(255, 0, 0);

  byte error, address;
  int nDevices;
  
  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    
     myGLCD.setFont(SmallFont);
    myGLCD.print("Searching for I2C devices, found:", 30, 150);

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
myGLCD.printNumI(address, 30 + nDevices*30, 170);
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);


    }    
  }
  if (nDevices == 0){
    Serial.println("No I2C devices found\n");
    myGLCD.print("No I2C devices found", 30, 150);
  }
  else
    Serial.println("done\n");

}

void initGraphics() {
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);


  myGLCD.clrScr();


  myGLCD.setColor(0, 255, 0);
}



void showPressure()
{
  
       myGLCD.setFont(BigFont);

  if (!bmp.begin()) {
      myGLCD.print("Could not find a valid BMP085 pressure sensor.", CENTER, 0);
      myGLCD.print("Anything connected at all ???", CENTER, 25);
  } else {
    myGLCD.setColor(255, 255, 255);
    myGLCD.printNumF(bmp.readPressure()/100, 1, CENTER, 40);
            myGLCD.print("mBar", CENTER, 55);

  }


}



