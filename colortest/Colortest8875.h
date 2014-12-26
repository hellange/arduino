#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
#include <Wire.h>
#include "Colortest.h"

#define RA8875_CS         10   // RA8875 chip select for ISP communication
#define RA8875_RESET      9    // Adafruit library puts a short low reset pulse at startup on this pin. 
                               // Not relevant for TFTM070 according to doc.            


class Colortest8875 : public Colortest{
 public:
  Colortest8875();

  Adafruit_RA8875 *tft;
  const static int height = 480;
  const static int width = 800;
  
  byte init();
  void drawRectangle(uint32_t color, int x1,int y1, int x2,int y2);
  void text();
};

