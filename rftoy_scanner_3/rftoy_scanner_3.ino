
#include <SPI.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// Based on Poor Man's Wireless 2.4GHz Scanner
// created March 2011 by Rolf Henkel
// 
// Modified by Helge Langehaug December 2014
// to work with RFToy.
// Uses Adafruit SSD1306 graphics library.

// PORTB = 8..13 digital   10 is bit2
// PORTC = 0..5 analog  
// Mirf.csnPin = 16;
// Mirf.cePin = 17;
// RFTOY PC2 analog ADC2  CSN  = PORTC bit 2
// RFTOY PC3 analog ADC3  CE = PORTC bit 3

// Array to hold Channel data
#define CHANNELS  127
int channel[CHANNELS];

// channel span
int stop_ch = CHANNELS;
int start_ch = 10;


// nRF24L01P registers we need
#define _NRF24_CONFIG      0x00
#define _NRF24_EN_AA       0x01
#define _NRF24_RF_CH       0x05
#define _NRF24_RF_SETUP    0x06
#define _NRF24_RPD         0x09

// get the value of a nRF24L01p register
byte getRegister(byte r)
{
 byte c;

 //PORTB &=~_BV(2); // pin 10
 PORTC &=~_BV(2);  

 c = SPI.transfer(r&0x1F);
 c = SPI.transfer(0);  
 
 //PORTB |= _BV(2);  // pin10
 PORTC |= _BV(2);   

 return(c);
}

// set the value of a nRF24L01p register
void setRegister(byte r, byte v)
{
 //PORTB &=~_BV(2);  // pin 10
 PORTC &=~_BV(2); 

 SPI.transfer((r&0x1F)|0x20);
 SPI.transfer(v);
 //PORTB |= _BV(2);  // pin 10
 PORTC |= _BV(2);  

}
 
// power up the nRF24L01p chip
void powerUp(void)
{
 setRegister(_NRF24_CONFIG,getRegister(_NRF24_CONFIG)|0x02);
 delayMicroseconds(130);
}

// switch nRF24L01p off
void powerDown(void)
{
 setRegister(_NRF24_CONFIG,getRegister(_NRF24_CONFIG)&~0x02);
}

// enable RX
void enable(void)
{
   PORTC |= _BV(3);
}

// disable RX
void disable(void)
{
   PORTC &=~_BV(3);
}

// setup RX-Mode of nRF24L01p
void setRX(void)
{
 setRegister(_NRF24_CONFIG,getRegister(_NRF24_CONFIG)|0x01);
 enable();
 // this is slightly shorter than
 // the recommended delay of 130 usec
 // - but it works for me and speeds things up a little...
 delayMicroseconds(100);
}

// scanning all channels in the 2.4GHz band
void scanChannels(void)
{
 disable();
 display.setTextSize(1);
 display.setTextColor(WHITE);
 bool show = true;
 int sweeps = 50;

 display.setCursor(0,0);
 display.print("Scanning");

 for( int j=0 ; j<sweeps  ; j++) 
 {
   if (j%5 == 0){
     /*
      display.drawLine(0, 0, 100, 0, BLACK);
      display.drawLine(0, 1, 100, 1, BLACK);
      display.drawLine(0, 2, 100, 2, BLACK);
      display.drawLine(0, 3, 100, 3, BLACK);
      display.drawLine(0, 4, 100, 4, BLACK);
      display.drawLine(0, 5, 100, 5, BLACK);
      display.drawLine(0, 6, 100, 6, BLACK);
      display.drawLine(0, 7, 100, 6, BLACK);


     if (show==true){
       show = false;
       display.print("               ");

     }
     else {
       show = true;
       display.print("Scanning ch:");
       display.print(j);
     }
     */
     display.drawLine(0, 10, j*50/sweeps, 10, WHITE);
   }
   display.display();

   int barHeight = 0;

   for( int i=start_ch ; i<stop_ch ; i++)
   {
     // select a new channel
     setRegister(_NRF24_RF_CH,(128*i)/CHANNELS);
     
     // switch on RX
     setRX();
     
     // wait enough for RX-things to settle
     delayMicroseconds(40);
     
     // this is actually the point where the RPD-flag
     // is set, when CE goes low
     disable();
     
     // read out RPD flag; set to 1 if
     // received power > -64dBm
     if( getRegister(_NRF24_RPD)>0 ){
       channel[i]++;
     //  barHeight = channel[i];
     //  if( barHeight>9 ) barHeight = 9;
     //  display.drawLine(i,53,i,53-barHeight*4,WHITE);
     }
   }
 }
}

// outputs channel data as a simple grey map
void outputChannels(void)
{
 int norm = 0;

 // find the maximal count in channel array
 for( int i=0 ; i<CHANNELS ; i++)
   if( channel[i]>norm ) norm = channel[i];

   display.clearDisplay();
   display.setCursor(0,56);
   display.print(start_ch);
   display.setCursor(110,56);
   display.print(stop_ch);
       
   for( int i=0 ; i<CHANNELS ; i++)
   {
     int pos;
   
     // calculate grey value position
     if( norm!=0 ) pos = (channel[i]*10)/norm;
     else          pos = 0;
   
     // boost low values
     if( pos==0 && channel[i]>0 ) pos++;
   
     // clamp large values
     if( pos>9 ) pos = 9;
       display.drawLine(i,53,i,53-pos*4,WHITE);

     channel[i] = 0;
   } 

   display.setCursor(70,0);
   display.print("Norm:");
   display.setCursor(100,0);
   display.print(norm);
   display.display();
}

void setup()
{
 Serial.begin(9600);
 
 Serial.println("Starting 2.4GHz Scanner ...");
 Serial.println();
 
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
 Serial.println("RFToy Display intialized ...");
 Serial.println();
 display.display();
    
 delay(500);
 display.clearDisplay();

 // Setup SPI
 SPI.begin();
 SPI.setDataMode(SPI_MODE0);
 SPI.setClockDivider(SPI_CLOCK_DIV2);
 SPI.setBitOrder(MSBFIRST);
 
 // analog pins for CSE and CE
 pinMode(16,OUTPUT);
 pinMode(17,OUTPUT);

 disable();
 
 // now start receiver
 powerUp();
 
 // switch off Shockburst
 setRegister(_NRF24_EN_AA,0x0);
 
 // make sure RF-section is set properly
 // - just write default value...
 setRegister(_NRF24_RF_SETUP,0x0F);

}

void loop()
{
 // do the scan
 scanChannels();
 
 // output the result
 outputChannels();
}




























 
