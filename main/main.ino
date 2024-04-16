#include <EtherCard.h>

#define led 4
char* ledStatus = "off";

//configura el buffer a 700 octetos o bytes
uint8_t Ethernet::buffer[700];

//Save MAC Address
static uint8_t mac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};

//SI ES POR IP ESTÁTICA DESCOMENTAR ESTAS LÍNEAS
/*
static uint8_t myip[] = {192,213,1,3};
static uint8_t gw[] = {192,213,1,1};
static uint8_t dns[] = {192,213,1,1};
static uint8_t msk[] = {255,255,255,0};
*/

//página HTML
static word homePage(){
  BufferFiller bfill = ether.tcpOffset(); //posiciona bfill adecuadamente en el buffer después de las cabeceras TCP/IP
  //emit_p escribe el contenido de la página web en el buffer
  bfill.emit_p(PSTR(
    //PSTR es un macro que guarda en la memoria de programa en vez de la RAM el siguiente texto
      "HTTP/1.0 200 OK\r\n"
      "Content-Type: text/html \r\n Pragma: no-cache \r\n Refresh: 5\r\n\r\n"
      "<html><meta charset='UTF-8'>"
      "<head><title>Equipo 4 - Servidor Ethernet</title></head>"
      "<body>"
      "<div style='text-align:center;'>"
      "<h1>Web server con Arduino UNO y módulo Ethernet ENC28J60</h1>"      
      "<br/><br/>Led status: $S <br/>"      
      "<a href=\"/?led=on\"><input type=\"button\" value=\"on\"></a>"
      "<a href=\"/?led=off\"><input type=\"button\" value=\"off\"></a>"
      "</div>"
      "</body></html>" 
  ),ledStatus);

  //devuelve el cursor o la posición en memoria de bfill
  return bfill.position();
}

void setup(){
  Serial.begin(9600);

  //iniciar el modulo
  //sizeof devuelve el número de bytes de un tipo de dato
  uint8_t vers = ether.begin(sizeof Ethernet::buffer, mac);

  //resultados de iniciar el módulo
  if(vers == 0){
    Serial.println("Error al iniciar el modulo");
  }
  else{
    Serial.println("Modulo iniciado correctamente");
  }

  //IP por DHCP
  if(!ether.dhcpSetup()){ //ether.dhcpSetup() configura la interfaz de red a DHCP
    Serial.println("DHCP exitoso");
  }
  else{   
    //ether.printlp() imprime en la terminal números en formato IP (separado por puntos)
    ether.printIp("IP: ", ether.myip); //IP
    ether.printIp("GW:   ", ether.gwip); //GATEWAY
    ether.printIp("Mask: ", ether.netmask); //NETMASK
    ether.printIp("DHCP server: ", ether.dhcpip); //DHCP SERVER
  }

  //SI SE HACE POR IP ESTÁTICA DESCOMENTAR ESTAS LÍNEAS Y COMENTAR
  //LÍNEAS 60-69
  /*
  //IP ESTÁTICA
  if(!ether.staticSetup(myip, gw, dns, msk)){ //configura la interfaz de red con datos estáticos
    Serial.println("IP statica correcta");
  }
  else{
    ether.printIp("IP: ", ether.myip); //IP
    ether.printIp("GW:   ", ether.gwip); //GATEWAY
    ether.printIp("Mask: ", ether.netmask); //NETMASK
  }
  */

  pinMode(led, OUTPUT);
}

void loop(){
  //recibe el paquete
  word len = ether.packetReceive();
  //busca el paquete en la memoria del buffer
  word pos = ether.packetLoop(len);

  if(pos){
    //busca en el buffer en la poscion pos la cadena "GET /?led=on"
    //char* Devuelve en texto lo que se encuentra en la memoria
    //Si es diferente de 0 (null) si encontró la subcadena
    if(strstr((char*) Ethernet::buffer + pos, "GET /?led=on") != 0){
      Serial.println("Led ON");
      digitalWrite(led, HIGH);
      ledStatus = "on";
    }

    if(strstr((char*) Ethernet::buffer + pos, "GET /?led=off") != 0){
      Serial.println("Led OFF");
      digitalWrite(led, LOW);
      ledStatus = "off";
    }

    //deuelve una respuesta HTTP 
    ether.httpServerReply(homePage());
  }
}