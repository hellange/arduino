#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>
#include <stdlib.h> // for malloc and free
//#include <new.h>

//void* operator new(size_t size) { return malloc(size); } 
//void operator delete(void* ptr) { free(ptr); }

byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):





class Clock
{
  private:
    int counter;
    int x_pos;
  public:
    Clock(){
      counter = 0;
      x_pos = 0;
    }
    void render(){
      GD.ColorA(160);
      
            GD.ColorRGB(0x000000);

      GD.cmd_text(190+x_pos/10 -1, 80-1, 31, OPT_CENTER, "Clock");
      GD.cmd_text(190+x_pos/10 +1, 80-1, 31, OPT_CENTER, "Clock");
      GD.cmd_text(190+x_pos/10 -1, 80+1, 31, OPT_CENTER, "Clock");
      GD.cmd_text(190+x_pos/10 +1, 80+1, 31, OPT_CENTER, "Clock");

      GD.ColorRGB(0xffffff);

      GD.cmd_text(190+x_pos/10, 80, 31, OPT_CENTER, "Clock");
      GD.ColorRGB(0x0000ff);
            GD.ColorA(255);

      GD.cmd_number(240, 60, 31, OPT_CENTER, counter ++);
      x_pos ++;
      if (x_pos>1000) x_pos = 0;
  }
};

class Weather
{

  public: 
  
    void render(){
      GD.ColorA(160);
      GD.cmd_text(240, 120, 31, OPT_CENTER, "Weather");
  }
};

Clock *clock;
Weather *weather;


void setup()
{
  GD.begin();
  GD.ClearColorRGB(0x103000);
  clock = new Clock();
  weather = new Weather();
}

void renderWeather(){
  GD.cmd_text(240, 80, 31, OPT_CENTER, "Weather");
}


void loop(){

  GD.ClearColorRGB(0x103000);
  GD.Clear();
  GD.PointSize(16*30);
  GD.Begin(POINTS);
  GD.ColorA(255);
  GD.ColorRGB(0x0080ff);
  GD.Vertex2ii(220,100);
  clock->render();
  weather->render();
  GD.swap();

}
