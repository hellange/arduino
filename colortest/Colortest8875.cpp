#include <SPI.h>

#include "Colortest8875.h"
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
 

#define RA8875_CS         10   // RA8875 chip select for ISP communication
#define RA8875_RESET      9    // Adafruit library puts a short low reset pulse at startup on this pin. 
                               // Not relevant for TFTM070 according to doc.            
Adafruit_RA8875 *tft;// = new Adafruit_RA8875(RA8875_CS, RA8875_RESET);

Colortest8875::Colortest8875(){
}

byte Colortest8875::init(){
  tft = new Adafruit_RA8875(RA8875_CS, RA8875_RESET);
  Serial.println("Init2");
  Serial.println("Trying to initialize RA8875 though SPI");
  if (!tft->begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }
  Serial.println("Found RA8875");
  tft->displayOn(true);
    Serial.println("Init3");

  tft->GPIOX(true);                              // Enable TFT - display enable tied to GPIOX
  tft->PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft->PWM1out(255);  
    Serial.println("Init4");

  tft->fillScreen(RA8875_RED);
    Serial.println("Init5");

}
/*
static int convertRGB888toRGB565(struct Buffer src, struct Buffer dst) {
    uint8_t *srcData, *dstData;
 
    srcData = (uint8_t *)src.data;
    dstData = (uint8_t *)dst.data;
 
    for (int c = 0, i = 0; c < src.width * src.height;) {
            //scale down each component to 5 or 6 bits
            uint8_t r, g, b;
            r = srcData[c++];
            g = srcData[c++];
            b = srcData[c++];
            r = (uint8_t)((srcData[c++]/255.0)*31); //R component
            g = (uint8_t)((srcData[c++]/255.0)*63); //G component
            b = (uint8_t)((srcData[c++]/255.0)*31); //B component
            dstData[i++] = ((r & 0x1f) << 3) | ((g >> 3) & 0x7); //R (5 bits) +  G (upper 3 bits)
            dstData[i++] = ((g & 0x7) << 5) | (b & 0x1f); //G (lower 3 bits) + B (5 bits)
    }
 
    return 0;
}
*/
 
void Colortest8875::drawRectangle(uint32_t color, int x1,int y1, int x2,int y2){
  
 // get RGB888 components
 unsigned char r = *((unsigned char *)&color +2);  
 unsigned char g = *((unsigned char *)&color +1);  
 unsigned char b = *((unsigned char *)&color);  
 
 // Convert to RGB565 (not sure if this the best/correct way to convert...)
 unsigned char r5 = (r * 249 + 1014) >> 11;
 unsigned char g6 = (g * 253 + 505) >> 10;
 unsigned char b5 = (b * 249 + 1014) >> 11;
 unsigned char res[2];
 res[0] = ((r5 & 0x1f) << 3) | ((g6 >> 3) & 0x7); //R (5 bits) +  G (upper 3 bits)
 res[1] = ((g6 & 0x7) << 5) | (b5 & 0x1f); //G (lower 3 bits) + B (5 bits)
 uint16_t rbg656 = res[0]<<8 | res[1];
 
 tft->fillRect(x1, y1, x2-x1, y2-y1,rbg656);
}

void Colortest8875::text(){
  tft->textMode();
  tft->textSetCursor(20, height / 4);
  tft->textTransparent(RA8875_WHITE);
  tft->textEnlarge(2);
  tft->textWrite("T e s t i n g   c o l o r s");
  
  tft->textSetCursor(20, 3 * height / 4);
  tft->textTransparent(RA8875_BLACK);
  tft->textEnlarge(2);
  tft->textWrite("T e s t i n g   c o l o r s");
  
}
