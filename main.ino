#include <EtherCard.h>

//Configure buffer size to 700 octets or bytes 
uint8_t Ethernet::buffer[700];

//Save MAC Address
static uint8_t mac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};

void setup(){
  Serial.begin(9600);

  //Start the module
  //sizeof return the number of bytes of a data type
  uint8_t vers = ether.begin(sizeof Ethernet::buffer, mac);

  //Results of starting the module
  if(vers == 0){
    Serial.println("Error starting the module");
  }
  else{
    Serial.println("Module started correctly");
  }

  //Triying to assing an IP from the DHCP server
  if(!ether.dhcpSetup()){ //ether.dhcpSetup() configures the network interface with DHCP
    Serial.println("DHCP wasn't successful");
  }
  else{
    //ether.printlp() Output to the serial port in dotted decimal IP
    ether.printIp("IP: ", ether.myip);
  }
}

void loop(){

}

