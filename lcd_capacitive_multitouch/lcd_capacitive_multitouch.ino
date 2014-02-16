/************************************************************************
 Capacitive multitouch demo for 7" LCD connected to Arduino.
 
 Written 15 February 2014 by Helge Langehaug.
 Depends on RA8875 library from Adafruit.

 Tested with ER-TFTM070-5 (LCD) from EastRising (bought from buydisplay.com)
 Follow the LCD manual and set resistors and jumpers according to
 4 wire SPI interface for RA8875.
 Connect LCD SPI to Arduino UNO SPI(using ICSP pins)
 Tested with R1-R3 open instead of 10Kohm as stated in manual.
 
 RA8875 communication, connect:
 
   TFTM070(40 pin)   Arduino UNO pin    Description
    1,2                                 GND
    3,4                                 VCC
    5                10                 SPI SELECT
    6                ICSP_MISO          SDO / SPI_MISO
    7                ICSP_MOSI          SDI / SPI_MOSI
    8                ICSP_SCK           CLK                
    9                NC
   33                2                  CTP_INT touch data ready for read (FROM FT5x06 touch controller)
   
   12 (why?)         9              RESET
                                    Everything works when connected to TFTM070 pin 12.
                                    It seems to be unstable without it. WHY ???
                                    Was originally though to go to TFTM070 pin 11 (reset).
                                    Was by mistake connected to TFTM070 pin 12. Luck ?  
                                    Why does this work ?
                                    Especially when the doc says that TFTM070 pin 11 and 12 are NC !!!
                                    Reset should not be needed at all because according to
                                    doc it is on-board.
                                    Soldering problem on my side ???
                                    To be examined !!!!


 Touch sensor FT5206 is preconfigured for I2C communication, connect:

   LCD(40) pin   Arduino UNO pin
   35            A4
   34            A5

*************************************************************************/

#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "Wire.h"
#include "FT5x06.h"


//                        Arduino pin
#define RA8875_CS         10
#define RA8875_RESET      9    // Adafruit library puts a short low reset pulse at startup on this pin
                               // Is to be connected to reset pin on RA8875.
#define CTP_INT           2    // touch data ready for read

#define SERIAL_DEBUG_ENABLED false  // set to true if you want debug info to serial port

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
FT5x06 cmt = FT5x06(CTP_INT);


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
    for (int i = 0;i < FT5206_NUMBER_OF_REGISTERS ; i++){
      Serial.print(registers[i],HEX);
      Serial.print(",");
    }
    Serial.println("");
}

void setup() 
{
  Serial.begin(9600);
  Serial.println("Trying to initialize RA8875 though SPI");
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

  Serial.println("Found RA8875");
  //delay(100);
  //cmt.printInfo();

  tft.displayOn(true);
  tft.GPIOX(true);                              // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);  
  tft.fillScreen(RA8875_GREEN);
  
  cmt.init(SERIAL_DEBUG_ENABLED);
  
  Serial.println("Setup done.");
}

word prev_coordinates[10]; // 5 pairs of x and y
byte nr_of_touches = 0;

void loop() 
{
    byte registers[FT5206_NUMBER_OF_REGISTERS];

    word coordinates[10];
    
    byte prev_nr_of_touches = 0;
    
    if (cmt.touched()){
    cmt.getRegisterInfo(registers);
    nr_of_touches = cmt.getTouchPositions(coordinates, registers);
    prev_nr_of_touches = nr_of_touches;

    if (SERIAL_DEBUG_ENABLED){
      printRawRegisterValuesToSerial(registers);
      serialDebugOutput(nr_of_touches, coordinates);
    }

    // remove previous touches on screen
    Serial.println(prev_nr_of_touches);
    for (int i = 0 ; i < prev_nr_of_touches; i++){
      word x = prev_coordinates[i * 2];
      word y = prev_coordinates[i * 2 + 1];
      tft.fillCircle(x, y, 70, RA8875_GREEN);
    }
    
    // mark touches on screen
    for (byte i = 0; i < nr_of_touches; i++){
      word x = coordinates[i * 2];
      word y = coordinates[i * 2 + 1];
      
      // Mark touches on screen
      tft.fillCircle(x, y, 70, RA8875_BLUE);
      tft.fillCircle(x, y, 50, RA8875_WHITE);
      tft.fillCircle(x, y, 30, RA8875_WHITE);
    }
    delay(10);
    memcpy(prev_coordinates, coordinates, 20);


    }
  
}
