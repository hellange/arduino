// Weather Station with LCD TFT display
// Uses several libraries, ref link at the #include lines
//
// Author:
// Helge Langehaug

#include <Wire.h>
#include <UTFT.h>            // http://www.henningkarlsen.com/electronics
#include <UTouch.h>          // http://www.henningkarlsen.com/electronics
#include <Adafruit_BMP085.h> // https://github.com/adafruit/Adafruit-BMP085-Library
#include <Timezone.h>        // https://github.com/JChristensen/Timezone
#include <DS1307RTC.h>       // http://playground.arduino.cc//Code/Time
#include <Time.h>            // http://playground.arduino.cc//Code/Time

#include "moon_phases_raw.h" 

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

// Using Arduino Mega and 3.2' TFT LCD 240x400 Display
UTFT myGLCD(TFT01_32WD,38,39,40,41);
UTouch myTouch(6,5,4,3,2);
Adafruit_BMP085 bmp;

boolean pressMode = false;
bool menuMode = false;

void setup(){
  Wire.begin();
  Serial.begin(9600);
  randomSeed(analogRead(0)); // ??
  
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
  
  // Uncomment to set date
  //setDateTime();
  
}


void loop(){
  myGLCD.setBackColor(0,0,0);
  renderDisplay();

  for (int i = 0; i<10; i++){
    checkTouch();
    delay(50);
  }
  
 
}

time_t lastPressureRender = now() - 1000L;
time_t lastPressureHistoryRender = now() - 1000L;
time_t lastMoonPhaseRender = now() - 1000L;

void renderDisplay(){
  
  if (now() - lastMoonPhaseRender > 5) {
    showMoonPhase(10,30);
    lastMoonPhaseRender = now();
  }

  if (now() - lastPressureRender > 5){
    showPressure();
    lastPressureRender = now();
  }
  if (now() - lastPressureHistoryRender > 3) {
    addHistoryValue(getMbar());
    lastPressureHistoryRender = now();
  }
  
  if (menuMode == false){
    showTime(40,150);
    showDate(60,210);
    showTemperature();
  } 
  
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
  myGLCD.setColor(10,50,10);
  myGLCD.fillRect(xaxis + x1, yaxis, xaxis + x2, yaxis-height);
  
  // top very visible
  myGLCD.setColor(50,220,50);
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


//******************* TOUCH ***************************
void checkTouch(){
  bool dataAvailable = myTouch.dataAvailable();
  if (dataAvailable && pressMode == false) {
    pressMode = true;
    int menuHeight=100;
    if (menuMode == false) {
       menuMode = true;
       myGLCD.setColor(0,0,0);
       myGLCD.fillRect(0,239,399,239-menuHeight);
       myGLCD.setColor(100,100,100);
       myGLCD.drawRect(0,239,399,239-menuHeight);

       myGLCD.setColor(0,255,0);
       myGLCD.setBackColor(0,0,0);
       //myGLCD.print("Menu On ", 170, 210);
       showMoonPhaseWeek(0,150);
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
void setDateTime(){
  // Use UTC
  int second =      30; //0-59
  int minute =      55; //0-59
  int hour =        20; //0-23
  int weekDay =     1; //1-7
  int monthDay =    22; //1-31
  int month =       12; //1-12
  int year  =       2013;
  
  setTime(hour,minute,second,monthDay,month,year);
  RTC.set(now());

}


const char* weekDays[] = {
                   "   \0",
                   "Sun\0",
                   "Mon\0",
                   "Tue\0",
                   "Wed\0",
                   "Thu\0",
                   "Fri\0",
                   "Sat\0"};
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
                   "Dec\0"};
                   
                   
//Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time +2h
TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 60};       //Central European Standard Time +1h
Timezone myTZ(CEST, CET);
time_t utc, local;

void showDate(int x,int y){
  myGLCD.setFont(SmallFont);
  myGLCD.setColor(150,150,150);

  myGLCD.print(weekDays[weekday()], x, y);
  myGLCD.printNumI(day(local), x+30, y, 2);
  myGLCD.print(monthNames[month(local)], x+55, y, 2);
  myGLCD.printNumI(year(local), x+90, y, 2);
}
void showTime(int x,int y){
  utc = now();
  TimeChangeRule *tcr; 
  local = myTZ.toLocal(utc, &tcr);
  char *timezone = tcr -> abbrev;

  myGLCD.setColor(100, 100, 200);
  myGLCD.setFont(SmallFont);
  myGLCD.print(timezone, 5, y);

  myGLCD.setFont(SevenSegNumFont);
  myGLCD.printNumI(hour(local), x, y, 2 ,'0');
  myGLCD.printNumI(minute(local), x+75, y, 2 ,'0');
  
  myGLCD.setFont(BigFont);
  myGLCD.printNumI(second(), x+140, y, 2 ,'0');
}

void showMoonPhase(int x,int y){
  char phaseText[30];
  // no conversion to local timezone, but...
  int phase = GetPhase(year(), month(), day(now()), phaseText);
  myGLCD.setColor(100,100,100);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Moon phase", x+5,y);  
  
  phaseText[17]='\0'; // show only first part of phase text
  myGLCD.print(phaseText, x, y+85);

  showMoonImage(phase,25,50);
}
void showMoonPhaseWeek(int x,int y){
  char phaseText[30];
  time_t oneDay =  60L*60L*24L; 
  // no conversion to local timezone, but...
  myGLCD.setColor(200,200,250);
  myGLCD.setFont(SmallFont);
  for (int i=0; i <= 5; i++){  
    time_t time = now() + oneDay * (i+1) * 2;
    int phase = GetPhase(year(time), month(time), day(time), phaseText);
    int xPos = x + 65*i;
    showMoonImage(phase, xPos ,y);
    myGLCD.print(monthNames[month(time)], xPos +10 , y + 70);
    myGLCD.printNumI(day(time), xPos + 40, y + 70);
    
    // horizontal marker
    myGLCD.setColor(150,150,200);
    myGLCD.drawRect(xPos, y+65, xPos + 60 , y+65);
  }
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
  }
}

float getMbar(){
  return bmp.readPressure()/100;
}

// *************** TEMP ******************
void showTemperature()
{
  myGLCD.setFont(BigFont);
  if (!bmp.begin()) {
    myGLCD.print("Could not find a valid BMP085 pressure sensor.", CENTER, 0);
    myGLCD.print("Anything connected at all ???", CENTER, 45);
  } else {
    
    int y = 150;
    myGLCD.setColor(0, 150, 0);
    myGLCD.setFont(SevenSegNumFont);
    float fTemp = bmp.readTemperature();
    myGLCD.printNumI((int)fTemp, 260, y);
    float dec = (fTemp - (int)fTemp) * 10; // first decimal
    myGLCD.setFont(BigFont);
    myGLCD.printNumI(dec, 335, y+35, 1);

    myGLCD.setFont(SmallFont);
    myGLCD.print("O", 330, y-5);
    myGLCD.setFont(BigFont);
    myGLCD.print("C", 340, y);

  }
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




void showMoonImage(int phase,int x,int y){
     
  prog_uint16_t *imgName[8];
  imgName[0] = new_;
  imgName[1] = waxing_crescent;
  imgName[2] = first_quarter;
  imgName[3] = waxing_gibbous;
  imgName[4] = full;
  imgName[5] = waning_gibbous;
  imgName[6] = last_quarter;
  imgName[7] = waning_crescent;
  
  
  myGLCD.drawBitmap(x,y,60,60,imgName[phase]);
  
  //myGLCD.drawBitmap(5,160,60,60,first_quarter);
}

