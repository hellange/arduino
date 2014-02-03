#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>
#include <stdlib.h> // for malloc and free
//#include <new.h>
#include <Ethernet.h>

//void* operator new(size_t size) { return malloc(size); } 
//void operator delete(void* ptr) { free(ptr); }

byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0xfe };
char server[] = "google.com";    // name address for Google (using DNS)


     EthernetClient client;


static void blocktext(int x, int y, byte font, const char *s)
{
  GD.SaveContext();
  GD.ColorRGB(0x000000);
  GD.cmd_text(x -1, y-1, 31, 0, s);
  GD.cmd_text(x +1, y-1, 31, 0, s);
  GD.cmd_text(x -1, y+1, 31, 0, s);
  GD.cmd_text(x +1, y+1, 31, 0, s);
  GD.RestoreContext();
  GD.cmd_text(x, y, 31, 0, "Clock");

}



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
      
      GD.ColorRGB(0x0000ff);
      GD.ColorA(255);

      blocktext(190+x_pos/10 -1, 80-1, 31, "Clock");

      GD.cmd_number(240, 60, 31, OPT_CENTER, counter ++);
      x_pos ++;
      if (x_pos>1000) x_pos = 0;
  }
};

class Weather
{

  public: 
  
    void render(){
      GD.ColorA(128);
      GD.ColorRGB(0xffffff);
      GD.cmd_text(240, 120, 31, OPT_CENTER, "Weather");
  }
};

class Network
{
   public:
     int a0,a1,a2,a3;
     bool hasAddress, hasConnection;
     Network(){
       hasAddress = false;
       hasConnection = false;
     }
     void getIpAddress(){
       if (Ethernet.begin(mac)==0){
          hasAddress=false;
       } else {  
           a0 = Ethernet.localIP()[0];
           a1 = Ethernet.localIP()[1];
           a2 = Ethernet.localIP()[2];
           a3 = Ethernet.localIP()[3];
           hasAddress=true;
       }
     }
     char *getStatus(){
       if (hasConnection == true){
         return "Connection successfull !";
       } 
       
       if (hasAddress == true){
         return "Got IP address, but connection failed !";
       }
       
       return "No network !";
         
       
     }
     
     
     void initNetwork(){
      Serial.begin(9600);
      Serial.println("Obtaining IP address...");
      getIpAddress();
      if (hasAddress){
        Serial.println("Got IP address");
      }
      
      delay(1000);
      Serial.println("connecting...");

      if (isConnected()){
        hasConnection = true;
        Serial.println("connected");
      } else {
        hasConnection = false;
        Serial.println("NOT connected");
      }
     }
     
     bool isConnected(){
       if (hasAddress && client.connect(server, 80)) {
         client.stop();
         Serial.println("Connecting ok");
         return true;
       } else {
         return false;
       }
     }
    
     
     void render(){
        if (hasAddress){
          GD.cmd_text(200, 200, 30, OPT_CENTER, "Ip address:");
          GD.cmd_number(200, 226, 29, OPT_CENTER, a0);
          GD.cmd_number(250, 226, 29, OPT_CENTER, a1);
          GD.cmd_number(300, 226, 29, OPT_CENTER, a2);
          GD.cmd_number(350, 226, 29, OPT_CENTER, a3);
        } 
        //else {
        //   GD.cmd_text(200, 200, 30, OPT_CENTER, "No connection !");
        //}
        
        GD.cmd_text(200, 250, 29, OPT_CENTER, getStatus());
        
     }
};

Clock *clock;
Weather *weather;
Network *network;

void setup()
{
    
  clock = new Clock();
  weather = new Weather();
  network = new Network();
  
  GD.begin();
  GD.ClearColorRGB(0x104000);
  GD.Clear();
  GD.cmd_text(200, 100, 30, OPT_CENTER, "Searching for network...");
  GD.swap();

  network->initNetwork();
  GD.ClearColorRGB(0x104000);
  GD.Clear();
  GD.cmd_text(200, 100, 30, OPT_CENTER, "Done searching...");
  GD.swap();

}

void renderWeather(){
  GD.cmd_text(240, 80, 31, OPT_CENTER, "Weather");
}


void loop(){

  GD.ClearColorRGB(0x104000);
  GD.Clear();
  GD.PointSize(16*30);
  GD.Begin(POINTS);
  GD.ColorA(255);
  GD.ColorRGB(0xff8000);
  GD.Vertex2ii(220,100);
  clock->render();
  weather->render();
  network->render();
  GD.swap();

}
