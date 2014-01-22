#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>
#include <Ethernet.h>

byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
void setup()
{
  GD.begin();
  GD.ClearColorRGB(0x103000);
  GD.Clear();
  GD.cmd_text(240, 50, 31, OPT_CENTER, "Searching for network...");
  GD.swap();
  
   // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // this check is only needed on the Leonardo:
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  //    GD.begin();

     //GD.ClearColorRGB(0x103000);
  //GD.Clear();
  GD.cmd_text(240, 136, 31, OPT_CENTER, "Failed to configure network");
  //GD.swap();
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // print your local IP address:
 /*
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 

  
  }
  */
  /*
  byte a = Ethernet.localIP()[0];
    byte b = Ethernet.localIP()[1];
  byte c = Ethernet.localIP()[2];
  byte d = Ethernet.localIP()[3];

  char *t = "Found network";
  char arr[10];
  strncpy(arr,t,10);
  */
  
 


   int a0 = Ethernet.localIP()[0];
  int a1 = Ethernet.localIP()[1];
  int a2 = Ethernet.localIP()[2];
  int a3 = Ethernet.localIP()[3];
  
   GD.cmd_text(200, 100, 30, OPT_CENTER, "Ip address:");

  GD.cmd_number(200, 136, 30, OPT_CENTER, a0);
  GD.cmd_number(250, 136, 30, OPT_CENTER, a1);
  GD.cmd_number(300, 136, 30, OPT_CENTER, a2);
  GD.cmd_number(350, 136, 30, OPT_CENTER, a3);
   GD.swap();

  GD.finish();
    GD.swap();

}

void loop()
{
 // GD.ClearColorRGB(0x103000);
 // GD.Clear();
 // GD.cmd_text(240, 136, 31, OPT_CENTER, "Hello world");
 // GD.swap();
 

  
 GD.swap();
}
