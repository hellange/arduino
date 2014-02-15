/************************************************************************
 Capacitive multitouch demo for 7" LCD connected to Arduino.
 
 Written 15 February 2014 by Helge Langehaug.
 Depends on RA8875 library from Adafruit.

 Tested with ER-TFTM070-5 from EastRising (bought from buydisplay.com)
 Follow the LCD manual and set resistors and jumpers according to
 4 wire SPI interface for RA8875.
 Connect LCD SPI to Arduino UNO SPI(using ICSP pins)
 Tested with R1-R3 open instead of 10Kohm as stated in manual.
 
 RA8875 communication, connect:
 
   LCD(40) pin   Arduino UNO pin    Description
    1,2                             GND
    3,4                             VCC
    5            10                 SPI SELECT
    6            ICSP_MISO          SDO / SPI_MISO
    7            ICSP_MOSI          SDI / SPI_MOSI
    8            ICSP_SCK           CLK                
    9            NC
   10            3                  INTERRUPT
   11            9                  RESET

 Touch sensor FT5206 is preconfigured for I2C communication, connect:

   LCD(40) pin   Arduino UNO pin
   35            A4
   34            A5

*************************************************************************/

#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "Wire.h"

/* FT5206 definitions */
#define FT5206_I2C_ADDRESS 0x38

#define FT5206_DEVICE_MODE 0x00

#define FT5206_GEST_ID 0x01
#define FT5206_GEST_ID_MOVE_UP     0x10
#define FT5206_GEST_ID_MOVE_LEFT   0x14
#define FT5206_GEST_ID_MOVE_DOWN   0x18
#define FT5206_GEST_ID_MOVE_RIGHT  0x1c
#define FT5206_GEST_ID_ZOOM_IN     0x48
#define FT5206_GEST_ID_ZOOM_OUT    0x49
#define FT5206_GEST_ID_NO_GESTURE  0x00

#define FT5206_TD_STATUS 0x02

#define FT5206_TOUCH1_XH 0x03
#define FT5206_TOUCH1_XL 0x04
#define FT5206_TOUCH1_YH 0x05
#define FT5206_TOUCH1_YL 0x06

#define FT5206_TOUCH2_XH 0x09
#define FT5206_TOUCH2_XL 0x0a
#define FT5206_TOUCH2_YH 0x0b
#define FT5206_TOUCH2_YL 0x0c

#define FT5206_TOUCH3_XH 0x0f
#define FT5206_TOUCH3_XL 0x10
#define FT5206_TOUCH3_YH 0x11
#define FT5206_TOUCH3_YL 0x12

#define FT5206_TOUCH4_XH 0x15
#define FT5206_TOUCH4_XL 0x16
#define FT5206_TOUCH4_YH 0x17
#define FT5206_TOUCH4_YL 0x18

#define FT5206_TOUCH5_XH 0x1b
#define FT5206_TOUCH5_XL 0x1c
#define FT5206_TOUCH5_YH 0x1d
#define FT5206_TOUCH5_YL 0x1e

//                   Arduino pin
#define RA8875_INT       3
#define RA8875_CS       10
#define RA8875_RESET     9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

byte getTouchPositions(word *touch_coordinates, byte *reg){
  
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

void setup() 
{
  Serial.begin(9600);
  Serial.println("Trying to initialize RA8875 though SPI");
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

  Serial.println("Found RA8875");

  /*
  // Interrupt
  pinMode(2,INPUT);
  attachInterrupt(0,gotit,FALLING);
  */

  tft.displayOn(true);
  tft.GPIOX(true);                              // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);  
  tft.fillScreen(RA8875_GREEN);
  
  Wire.begin();
  Wire.beginTransmission(FT5206_I2C_ADDRESS);
  Wire.write(FT5206_DEVICE_MODE);
  Wire.write(0);
  Wire.endTransmission(FT5206_I2C_ADDRESS);
  
  Serial.println("Setup done.");
}

void serialDebugOutput(int i,word x,word y){
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

void loop() 
{
    Wire.requestFrom(0x38,40); 
    char buffer[50];
    byte registers[50];
    int pos = 0;

    // get all register bytes when available
    while(Wire.available())
    {
      registers[pos++] = Wire.read();
    }

    // raw register values
    /*
    for (int i =0;i<40;i++){
      Serial.print(reg[i],HEX);
      Serial.print(",");
    }
    //  Serial.println("");
    */
 
    word coordinates[10]; // 5 pairs of x and y
    byte nr_of_touches = getTouchPositions(coordinates,registers);
    
    word x;
    word y;
    for (byte i = 0; i < nr_of_touches; i++){
      x = coordinates[i * 2];
      y = coordinates[i * 2 + 1];
      
      // Show coordinates on serial
      serialDebugOutput(i, x, y);
      if (i == nr_of_touches - 1){
        Serial.println(" ");
      }
      
      // Mark touches on screen
      tft.fillCircle(x, y, 70, RA8875_BLUE);
      tft.fillCircle(x, y, 50, RA8875_RED);
      tft.fillCircle(x, y, 30, RA8875_WHITE);
    }

   
    delay(100);
    for (int i = 0 ; i < nr_of_touches; i++){
      x = coordinates[i * 2];
      y = coordinates[i * 2 + 1];
      tft.fillCircle(x, y, 70, RA8875_GREEN);
    }
  
}
