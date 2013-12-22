// Weather Station with LCD TFT display
// Based on 
//   Graphics library (C)2012 Henning Karlsen
//   Adafruit_BMP085 by Adafruit
//   i2c_scanner from various sources...
// Author:
// Helge Langehaug

#include <UTFT.h>    // http://www.henningkarlsen.com/electronics
#include <UTouch.h>  // http://www.henningkarlsen.com/electronics
#include <Adafruit_BMP085.h> // https://github.com/adafruit/Adafruit-BMP085-Library
#include <Wire.h>
#include "moon_phases_raw.h" 

#include <DS1307RTC.h> // http://playground.arduino.cc//Code/Time
#include <Time.h>      // http://playground.arduino.cc//Code/Time
//
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

// Using Arduino Mega and 3.2' TFT LCD 240x400 Display
UTFT myGLCD(TFT01_32WD,38,39,40,41);
UTouch myTouch(6,5,4,3,2);
int counter = 0;
int x, y;
Adafruit_BMP085 bmp;

//#define DS1307_ADDRESS 0x68
byte zero = 0x00;
int p= 1000;
int delta = 1;
void setup(){
  Wire.begin();
  Serial.begin(9600);
  randomSeed(analogRead(0)); // ??
  //setDateTime();
  printDate();
  initGraphics();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_LOW);
  //myTouch.setPrecision(PREC_MEDIUM);
  //myTouch.setPrecision(PREC_HI);
  //myTouch.setPrecision(PREC_EXTREME);

  // the function to get the time from the RTC
  setSyncProvider(RTC.get);   
  
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");      

  printDate();
   
  
  //barGraph();
  //i2c_scanner();
}

bool menuMode = false;

int secondSinceLastHistory = 9999;
void loop(){
  myGLCD.setBackColor(0,0,0);

  printDate();
  showPressure();
  
  for (int i = 0; i<18; i++){
    checkTouch();
    delay(50);
  }
  
  // every 30 minutes
  if (secondSinceLastHistory > 60 * 30) {
    secondSinceLastHistory = 0;
    addHistoryValue(getMbar());
  }
  secondSinceLastHistory += 1;
  /*
  if (p>1040){
    delta = -3;
  } else if (p<960) {
     delta = 4;
  }
    p=p+delta;
  */ 
}

//******************* GRAPHICS INIT ***************************
void initGraphics() {
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  myGLCD.fillScr(0,0,0);
  //myGLCD.setBackColor(240,240,240);
  myGLCD.setColor(0, 255, 0);
  
  
  
}

//******************* BARGRAPH ************************
void drawBar(int index, int value, int valueOffset){
  int maxValue = 100;
  
  // lower left corner coordinates for the bargrah
  int yaxis = 105;
  int xaxis = 110;
  
  int margin = 2;
  int width = 6;
  
  int height = value - valueOffset;
  if (height < 1){
    height = 0;
  }
  //int height = value - valueOffset;

  int x1 = index * width;
  int x2 = x1 + width - margin;
  
  
  // given value range (after offset) is between 0 and 100 (950-1050)
  // and display height = 100, adjust to full display range
  height = height; //   /0.8;

  
  // draw it
  myGLCD.setColor(30,100,30);
  myGLCD.fillRect(xaxis + x1, yaxis, xaxis + x2, yaxis-height);
  
  // top very visible
  myGLCD.setColor(100,200,100);
  int markerHeight = 5;
  if (height < markerHeight){
    markerHeight = height;
  }
  myGLCD.fillRect(xaxis + x1, yaxis-height, xaxis + x2, yaxis-height+markerHeight);
  
  // clear top (to replace previous bar if it was higher)
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(xaxis + x1, yaxis - maxValue, xaxis + x2, yaxis-height);

  // border
  myGLCD.setColor(10,10,10);
  myGLCD.drawRect(xaxis, yaxis, xaxis + 24 * width, yaxis - maxValue);



}


int history[] = {950,1000,1050,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void addHistoryValue(int value){

  // shift left
  for (int i = 1; i<24; i++){
    history[i-1] = history[i];
  }
  history[23] = value;
  
  // given max history 1040, min history 960, total of 80 points
  // convert to bar scale
  //int bars[24];
  //for (int i = 1; i<24; i++){
  //  bars[i] = history[i] - 960; // given 950 is the lowest value we can possibly have in history
  //}
  
  drawMillibarGraph(history, 24);
}


void drawMillibarGraph(int values[], int nr_of_values){
  
  for (int i = 0; i< nr_of_values; i++){
    drawBar(i,values[i], 950); // value offset 950 gives us 0 in graph for 950mBar
  }
}

boolean pressMode = false;
//******************* TOUCH ***************************
void checkTouch(){
  bool dataAvailable = myTouch.dataAvailable();
  if (dataAvailable && pressMode == false) {
    pressMode = true;
    int menuHeight=120;
    if (menuMode == false) {
       menuMode = true;
       myGLCD.setColor(100,100,100);
       myGLCD.fillRect(0,240,399,240-menuHeight);
       
       myGLCD.setColor(0,255,0);
       myGLCD.setBackColor(100,100,100);
       myGLCD.print("Menu On ", 170, 210);

    } else {
      menuMode = false;
       myGLCD.setColor(0,0,0);
       myGLCD.fillRect(0,240,399,240-menuHeight);
       
       myGLCD.setColor(0,255,0);
       myGLCD.setBackColor(0,0,0);
       //myGLCD.print("Menu Off", 170, 210);
    }
  } else if (dataAvailable && pressMode == true){
    // not released yet...
  } else {
    // released...
    pressMode = false;
  }
  /*
  else {
      myTouch.read();
      myGLCD.setColor(0,255,0);
      x=myTouch.getX();
      y=myTouch.getY();
      myGLCD.printNumI(x, 0, 0, 3);
      myGLCD.printNumI(y, 50, 0, 3);
      myGLCD.fillCircle(x,y,10);
  }
  */
}

//******************* CLOCK/DATE ***********************
byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}
byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}
/*
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
*/

const char* weekDays[] = {
                   "         \0",
                   "   Sunday\0",
                   "   Monday\0",
                   "  Tuesday\0",
                   "Wednesday\0",
                   " Thursday\0",
                   "   Friday\0",
                   " Saturday\0"};
const char* monthNames[] = {
                   "   \0",
                   "Jan\0",
                   "Feb\0",
                   "Mar\0",
                   "Apr\0",
                   "May\0",
                   "Jun\0",
                   "Jul\0",
                   "Aug\0",
                   "Sep\0",
                   "Oct\0",
                   "Nov\0",
                   "Des\0"};
void printDate(){

  // Reset the register pointer
 // Wire.beginTransmission(DS1307_ADDRESS);

 // Wire.write(zero);
 // Wire.endTransmission();

 // Wire.requestFrom(DS1307_ADDRESS, 7);
  /*
  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());
  */
 /*
  int second = second();
  int minute = minute();
  int hour = hour();
  int weekDay = weekday();
  int monthDay = day();
  int month = month();
  int year = year();
*/


  /* Another calculation, seems to work */
  char phaseText[30];
  int phase = GetPhase(year(), month(), day(), phaseText);
  myGLCD.setColor(100,100,100);
  myGLCD.print("Moon:", 10,10);
  myGLCD.printNumI(phase, 10, 30);
  
  myGLCD.setFont(SmallFont);
  
  phaseText[17]='\0'; // show only first part of phase text
  myGLCD.print(phaseText, 10, 115);

  showBitmap(phase);

  myGLCD.setFont(SevenSegNumFont);

  y = 150;
  myGLCD.setColor(100, 100, 200);
  myGLCD.printNumI(hour(), 30, y, 2 ,'0');
  //myGLCD.print(":", 30+, y);
  myGLCD.printNumI(minute(), 30+75, y, 2 ,'0');
  
  myGLCD.setFont(BigFont);
  myGLCD.printNumI(second(), 180, y, 2 ,'0');
  
  myGLCD.setFont(SmallFont);
  
  int y = 220;
  int x = 175;
  myGLCD.setColor(150,150,150);
  
  myGLCD.print(weekDays[weekday()], x, y);
  myGLCD.printNumI(day(), x+80, y, 2);
  //myGLCD.print("/", y+100, y);
  myGLCD.print(monthNames[month()], x+105, y, 2);
  //myGLCD.print("/", y+160, y);
  myGLCD.printNumI(year(), x+140, y, 2);
  

}




//******************* I2C SCANNING ***********************
void i2c_scanner()
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


//******************* PRESSURE ***************************
void showPressure()
{
  myGLCD.setFont(BigFont);
  if (!bmp.begin()) {
    myGLCD.print("Could not find a valid BMP085 pressure sensor.", CENTER, 0);
    myGLCD.print("Anything connected at all ???", CENTER, 45);
  } else {
    myGLCD.setColor(0, 150, 0);
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.printNumI((int)getMbar(), 260, 40);
    myGLCD.setFont(BigFont);
    myGLCD.setColor(80, 120, 80);
    myGLCD.print("mBar", 260, 90);
    
    myGLCD.setColor(0, 150, 0);
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.printNumI((int)bmp.readTemperature(), 260, 90+40);
    
    myGLCD.setFont(SmallFont);
    myGLCD.print("O", 330, 125);
    myGLCD.setFont(BigFont);
    myGLCD.print("C", 340, 130);

   // myGLCD.setFont(BigFont);
   // myGLCD.setColor(80, 120, 80);
   // myGLCD.print("Celsius", 260, 140+40);



  }
}

float getMbar(){
  return bmp.readPressure()/100;
}

// Ref:http://www.nano-reef.com/topic/217305-a-lunar-phase-function-for-the-arduino/
int GetPhase(int nYear, int nMonth, int nDay, char *phaseText) // calculate the current phase of the moon
{
int phase;
double AG, IP;
long YY, MM, K1, K2, K3, JD;
YY = nYear - floor((12 - nMonth) / 10);
MM = nMonth + 9;
if (MM >= 12)
{
MM = MM - 12;
}
K1 = floor(365.25 * (YY + 4712));
K2 = floor(30.6 * MM + 0.5);
K3 = floor(floor((YY / 100) + 49) * 0.75) - 38;
JD = K1 + K2 + nDay + 59;
if (JD > 2299160)
{
JD = JD - K3;
}
IP = MyNormalize((JD - 2451550.1) / 29.530588853);
AG = IP*29.53;
phase = 0;
if ((AG < 1.84566) && (phase == 0))
{
//phase = 0; //new; 0% illuminated
phase = 0;
strcpy(phaseText,"New              XXXXXXXXXX");
}
if ((AG < 5.53699) && (phase == 0))
{
//phase = .25; //Waxing crescent; 25% illuminated
phase = 1;
strcpy(phaseText,"Waxing crescent  XXXXXX__");
}
if ((AG < 9.922831) && (phase == 0))
{
//phase = .50; //First quarter; 50% illuminated
phase = 2;
strcpy(phaseText,"First quarter    XXXX____");

}
if ((AG < 12.91963) && (phase == 0))
{
//phase = .75; //Waxing gibbous; 75% illuminated
phase = 3;
strcpy(phaseText,"Waxing gibbous   XX______");

}
if ((AG < 16.61096) && (phase == 0))
{
//phase = 1; //Full; 100% illuminated
phase = 4;
strcpy(phaseText,"Full             ________");

}
if ((AG < 20.30228) && (phase == 0))
{
//phase = .75; //Waning gibbous; 75% illuminated
phase = 5;
strcpy(phaseText,"Waning gibbous   ______XX");

}
if ((AG < 23.99361) && (phase == 0))
{
//phase = .50; //Last quarter; 50% illuminated
phase = 6;
strcpy(phaseText,"Last quarter     ____XXXX");

}
if ((AG < 27.68493) && (phase == 0))
{
phase = 7;
//phase = .25; //Waning crescent; 25% illuminated
strcpy(phaseText,"Waning crescent  __XXXXXX");

}
if (phase == 0)
{
//phase = 0; //default to new; 0% illuminated
phase = 0;
strcpy(phaseText,"New              XXXXXXXX");

}
return phase;
}

double MyNormalize(double v)
{
v = v - floor(v);
if (v < 0)
v = v + 1;
return v;
} 




void showBitmap(int phase){
     
  prog_uint16_t *imgName[8];
  imgName[0] = new_;
  imgName[1] = waxing_crescent;
  imgName[2] = first_quarter;
  imgName[3] = waxing_gibbous;
  imgName[4] = full;
  imgName[5] = waning_gibbous;
  imgName[6] = last_quarter;
  imgName[7] = waning_crescent;
  
  
  myGLCD.drawBitmap(25,50,60,60,imgName[phase]);
  
  //myGLCD.drawBitmap(5,160,60,60,first_quarter);
}

