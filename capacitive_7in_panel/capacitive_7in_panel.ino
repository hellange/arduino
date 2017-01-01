/***************************************************************************************
 7" Capacitive multitouch demo for Arduino
 
 Written 1 Jan 2017 by Helge Langehaug.

 Tested with ER-TPC070-6 (7 inch Capacitive Touch Panel with Controller FT5316 for 800x480) 
 from EastRising (bought from buydisplay.com)

 Connections:

 ER-TPC070-6     UNO

 1                           GND
 2               A4          SDA
 3               A5          SCL
 4                           RESET
 5               2           INT
 6                           NC
 7               3.3V        VDD
 8                           VDD
 9                           GND
 10              GND         GND

****************************************************************************************/

#include <SPI.h>
#include "Wire.h"
#include "FT5xx6.h"
                               
#define CTP_INT           2    // touch data ready for read from touch panel
FT5xx6 cmt = FT5xx6(CTP_INT);

void serialDebugOutput(int nr_of_touches, word *coordinates) {
  for (byte i = 0; i < nr_of_touches; i++){

    word x = coordinates[i * 2];
    word y= coordinates[i * 2 + 1];
    
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
}

void printRawRegisterValuesToSerial(byte *registers) {
    // print raw register values
    for (int i = 0;i < FT5xx6_NUMBER_OF_REGISTERS ; i++){
      Serial.print(registers[i],HEX);
      Serial.print(",");
    }
    Serial.println("");
}

void setup() 
{
  Serial.begin(9600);
  cmt.init(true);
}

word prev_coordinates[10]; // 5 pairs of x and y
byte nr_of_touches = 0;

void loop() 
{
    byte registers[FT5xx6_NUMBER_OF_REGISTERS];
    word coordinates[10];
        
    if (cmt.touched()){
      cmt.getRegisterInfo(registers);
      nr_of_touches = cmt.getTouchPositions(coordinates, registers);

      serialDebugOutput(nr_of_touches, coordinates);
      printRawRegisterValuesToSerial(registers);
      
      delay(10);
    }
  
}
