/************************************************************************
    @file     FT5x06.cpp
    @author   Helge Langehaug
    @license  BSD license, all text above and below must be included in
              any redistribution

This is a library to handling capacitive multitouch sensors using FT5x06.
Originally written to work with ER-TFTM070-5 (LCD module) from EastRising.
*************************************************************************/

#include <SPI.h>
#include "Wire.h"
#include "FT5x06.h"


  FT5x06::FT5x06(){
  }
  byte FT5x06::getTouchPositions(word *touch_coordinates, byte *reg){
    
    byte nr_of_touches = reg[FT5206_TD_STATUS] & 0xF;
    if (nr_of_touches > 0){
      touch_coordinates[0] = word(reg[FT5206_TOUCH1_XH] & 0x0f,reg[FT5206_TOUCH1_XL]);
      touch_coordinates[1] = word(reg[FT5206_TOUCH1_YH] & 0x0f,reg[FT5206_TOUCH1_YL]);
    }
    if (nr_of_touches > 1){
      touch_coordinates[2] = word(reg[FT5206_TOUCH2_XH] & 0x0f,reg[FT5206_TOUCH2_XL]);
      touch_coordinates[3] = word(reg[FT5206_TOUCH2_YH] & 0x0f,reg[FT5206_TOUCH2_YL]);
    }
    if (nr_of_touches > 2){
      touch_coordinates[4] = word(reg[FT5206_TOUCH3_XH] & 0x0f,reg[FT5206_TOUCH3_XL]);
      touch_coordinates[5] = word(reg[FT5206_TOUCH3_YH] & 0x0f,reg[FT5206_TOUCH3_YL]);
    }
    if (nr_of_touches > 3){
      touch_coordinates[6] = word(reg[FT5206_TOUCH4_XH] & 0x0f,reg[FT5206_TOUCH4_XL]);
      touch_coordinates[7] = word(reg[FT5206_TOUCH4_YH] & 0x0f,reg[FT5206_TOUCH4_YL]);
    }
    if (nr_of_touches > 4){
      touch_coordinates[8] = word(reg[FT5206_TOUCH5_XH] & 0x0f,reg[FT5206_TOUCH5_XL]);
      touch_coordinates[9] = word(reg[FT5206_TOUCH5_YH] & 0x0f,reg[FT5206_TOUCH5_YL]);
    }
    return nr_of_touches;
  }

  /*
  // interrupts for touch detection
  volatile int value = 0;
  void gotit(){
    value++;
  }
  */

  void FT5x06::init() {
    Serial.begin(9600);
    Serial.println("Trying to initialize FT5x06 by I2C");

    /*
    // Interrupt
    pinMode(2,INPUT);
    attachInterrupt(0,gotit,FALLING);
    */

  
    Wire.begin();
    Wire.beginTransmission(FT5206_I2C_ADDRESS);
    Wire.write(FT5206_DEVICE_MODE);
    Wire.write(0);
    Wire.endTransmission(FT5206_I2C_ADDRESS);
  
    Serial.println("Setup done.");
  }

  void FT5x06::serialDebugOutput(int i,word x,word y) {
    Serial.print("x");
    Serial.print(i);
    Serial.print("=");
    Serial.print(x);
    Serial.print(",");
    Serial.print("y");
    Serial.print(i);
    Serial.print("=");
    Serial.print(y);
    Serial.print("  ");
  }

  void FT5x06::getRegisterInfo(byte *registers) {
    int pos = 0;
    // get all register bytes when available
    while(Wire.available())
    {
      registers[pos++] = Wire.read();
    }
  }


