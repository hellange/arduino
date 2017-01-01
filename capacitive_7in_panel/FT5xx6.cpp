/*************************************************************************************
This is a library to handling capacitive multitouch panel with FT5xx6 controller
Originally written to work with ER-TFTM070-5 (LCD module with FT5206 touch controller) 
from EastRising.
Now also tested with ER-TPC070-6 7" Capacitive Touch Panel (FT5316 for 800x480)
Not tested with other controllers.

Written by Helge Langehaug, 2014 - 2017

BSD license, all text above must be included in any redistribution
*************************************************************************/

#include <SPI.h>
#include "Wire.h"
#include "FT5xx6.h"

  FT5xx6::FT5xx6(uint8_t CTP_INT){
    _ctpInt = CTP_INT;
  }
  
  byte FT5xx6::getTouchPositions(word *touch_coordinates, byte *reg){
    
    byte nr_of_touches = reg[FT5xx6_TD_STATUS] & 0xF;
    if (nr_of_touches > 0){
      touch_coordinates[0] = word(reg[FT5xx6_TOUCH1_XH] & 0x0f,reg[FT5xx6_TOUCH1_XL]);
      touch_coordinates[1] = word(reg[FT5xx6_TOUCH1_YH] & 0x0f,reg[FT5xx6_TOUCH1_YL]);
    }
    if (nr_of_touches > 1){
      touch_coordinates[2] = word(reg[FT5xx6_TOUCH2_XH] & 0x0f,reg[FT5xx6_TOUCH2_XL]);
      touch_coordinates[3] = word(reg[FT5xx6_TOUCH2_YH] & 0x0f,reg[FT5xx6_TOUCH2_YL]);
    }
    if (nr_of_touches > 2){
      touch_coordinates[4] = word(reg[FT5xx6_TOUCH3_XH] & 0x0f,reg[FT5xx6_TOUCH3_XL]);
      touch_coordinates[5] = word(reg[FT5xx6_TOUCH3_YH] & 0x0f,reg[FT5xx6_TOUCH3_YL]);
    }
    if (nr_of_touches > 3){
      touch_coordinates[6] = word(reg[FT5xx6_TOUCH4_XH] & 0x0f,reg[FT5xx6_TOUCH4_XL]);
      touch_coordinates[7] = word(reg[FT5xx6_TOUCH4_YH] & 0x0f,reg[FT5xx6_TOUCH4_YL]);
    }
    if (nr_of_touches > 4){
      touch_coordinates[8] = word(reg[FT5xx6_TOUCH5_XH] & 0x0f,reg[FT5xx6_TOUCH5_XL]);
      touch_coordinates[9] = word(reg[FT5xx6_TOUCH5_YH] & 0x0f,reg[FT5xx6_TOUCH5_YL]);
    }
    return nr_of_touches;
  }


  // interrupts for touch detection
  volatile bool newTouch = false;
  
  void touch_interrupt(){
    newTouch = true;
  }
  
  bool FT5xx6::touched(){
    if (newTouch == true){
      newTouch = false;
      return true;
    } else {
      return false;
    }
  }

  void FT5xx6::init(bool serial_output_enabled) {
    
    if(serial_output_enabled){
      Serial.begin(9600);
      Serial.println("Trying to initialize FT5xx6 by I2C");
    }

    
    // Interrupt
    pinMode(_ctpInt ,INPUT);
    #ifdef digitalPinToInterrupt
     attachInterrupt(digitalPinToInterrupt(_ctpInt),touch_interrupt,FALLING);
    #else
      attachInterrupt(0,touch_interrupt,FALLING);
    #endif   

    Wire.begin();
    Wire.beginTransmission(FT5xx6_I2C_ADDRESS);
    Wire.write(FT5xx6_DEVICE_MODE);
    Wire.write(0);
    Wire.endTransmission(FT5xx6_I2C_ADDRESS);
  
    if(serial_output_enabled){
      Serial.println("Setup done.");
    } 
  }

  void FT5xx6::getRegisterInfo(byte *registers) {
    Wire.requestFrom(FT5xx6_I2C_ADDRESS, FT5xx6_NUMBER_OF_REGISTERS); 
    int register_number = 0;
    // get all register bytes when available
    while(Wire.available())
    {
      registers[register_number++] = Wire.read();
    }
  }
  
  void FT5xx6::printInfo(){
    byte registers[FT5xx6_NUMBER_OF_TOTAL_REGISTERS];
    Wire.requestFrom(FT5xx6_I2C_ADDRESS, FT5xx6_NUMBER_OF_TOTAL_REGISTERS); 
    int register_number = 0;
    // get all register bytes when available
    while(Wire.available())
    {
      registers[register_number++] = Wire.read();
      delay(5);
    }
    delay(10);
    // Might be that the interpretation of high/low bit is not same as major/minor version...
    Serial.print("Library version: ");
    Serial.print(registers[FS5xx6_TOUCH_LIB_VERSION_H]);
    Serial.print(".");
    Serial.print(registers[FS5xx6_TOUCH_LIB_VERSION_L]);
    Serial.println(".");

  }


