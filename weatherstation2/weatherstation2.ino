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

//
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

// Using Arduino Mega and 3.2' TFT LCD 240x400 Display
UTFT myGLCD(TFT01_32WD,38,39,40,41);
UTouch myTouch(6,5,4,3,2);
int counter = 0;
int x, y;
Adafruit_BMP085 bmp;

#define DS1307_ADDRESS 0x68
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

  printDate();
   
  
  //barGraph();
  //i2c_scanner();
}

int secondSinceLastHistory = 9999;
void loop(){
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
  myGLCD.fillScr(240,240,240);
  myGLCD.setBackColor(240,240,240);
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
  
  
  // given value range (after offset) is between 0 and 80
  // and display height = 100, adjust to full display range
  height = height/0.8;

  
  // draw it
  myGLCD.setColor(0,0,255);
  myGLCD.fillRect(xaxis + x1, yaxis, xaxis + x2, yaxis-height);
  
  // clear top (to replace previous bar if it was higher)
  myGLCD.setColor(240,240,240);
  myGLCD.fillRect(xaxis + x1, yaxis - maxValue, xaxis + x2, yaxis-height);

  myGLCD.setColor(0,0,150);
  myGLCD.drawRect(xaxis, yaxis, xaxis + 24 * width, yaxis - maxValue);



}


int history[] = {960,1000,1040,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
    drawBar(i,values[i], 960); // value offset 960 gives us 0 in graph for 960mBar
  }
}


//******************* TOUCH ***************************
void checkTouch(){
  if (myTouch.dataAvailable()) {
    myTouch.read();
      myGLCD.setColor(0,255,0);
      x=myTouch.getX();
      y=myTouch.getY();
      myGLCD.printNumI(x, 0, 0, 3);
      myGLCD.printNumI(y, 50, 0, 3);
      myGLCD.fillCircle(x,y,10);
  }
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
  /*
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
  */
  myGLCD.setFont(BigFont);

  int y = 150;
  myGLCD.setColor(150,150,150);
  myGLCD.printNumI(monthDay, 120, y, 2);
  myGLCD.print("/", 150, y);
  myGLCD.printNumI(month, 165, y, 2);
  myGLCD.print("/", 195, y);
  myGLCD.printNumI(year, 210, y, 2);
  
  y = 130;
  myGLCD.setColor(0, 0, 0);
  myGLCD.printNumI(hour, 120, y, 2 ,'0');
  myGLCD.print(":", 150, y);
  myGLCD.printNumI(minute, 165, y, 2 ,'0');
  myGLCD.print(":", 195, y);
  myGLCD.printNumI(second, 210, y, 2 ,'0');
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
    myGLCD.setColor(0, 0, 255);
    myGLCD.printNumF(getMbar(), 1, 270, 30);
    myGLCD.print("mBar", 270, 45);
    
    myGLCD.print("Temp:", 270, 65);
    myGLCD.printNumF(bmp.readTemperature(), 1, 270, 80);
    byte degree = 176;
    myGLCD.print(degree, 1, 380, 80);

  }
}

float getMbar(){
  return bmp.readPressure()/100;
}



