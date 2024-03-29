#include <Adafruit_CC3000.h>
#include <Adafruit_CC3000_Server.h>
#include <ccspi.h>

//#include <Adafruit_CC3000.h>
//#include <ccspi.h>
#include <SPI.h>
#include "cc3000_PubSubClient.h"

#include "iotapi_client.h"
#include "config.h"
#include <string.h>
#include <NewPing.h>


//Config sensor
#define TRIGGER_PIN_S1  2// Arduino pin tied to trigger pin on the ultrasonic sensor.
#define TRIGGER_PIN_S2  8
#define MAX_DISTANCE 250 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar1(TRIGGER_PIN_S1, TRIGGER_PIN_S1, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN_S2, TRIGGER_PIN_S2, MAX_DISTANCE);


//IP por partes
unsigned char octet[4] = {0,0,0,0};
unsigned int prue[4] = {0,0,0,0};

//Some functions
void sendMessage(char* topic, char* message);

void callback (char* topic, byte* payload, unsigned int length) {
}

cc3000_PubSubClient mqttclient(server.ip, MQTT_PORT, callback, client, cc3000);


//Start
void setup(void)
{
    Serial.begin(115200);
    Serial.println(F("Hello, CC3000!\n"));
    Serial.println(F("\nInitialising the CC3000 ..."));

  if (!cc3000.begin()) {
      Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
  }else{
      Serial.println(F("Arrancado!"));
  }

  /* Attempt to connect to an access point */
    char *ssid = WLAN_SSID;             /* Max 32 chars */
    Serial.print(F("\nAttempting to connect to "));
    Serial.println(ssid);

  /* NOTE: Secure connections are not available in 'Tiny' mode! */
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
      Serial.println(F("Failed!"));
    /*while(1);*/
  } else {
    Serial.println(F("Connected to WiFi!"));
  }


  // Display the IP address DNS, Gateway, etc.
  while (!displayConnectionDetails2()) {
    delay(1000);
  }
    Serial.println(F("Connected to server ?"));
  // connect to the broker

  //Serial.println(F(server.ip));
  if (!client.connected()) {
      Serial.println("Connecting to TCP server...");
      client = cc3000.connectTCP(server.ip, 1883);
      Serial.println("Server connected");
  } else {
      Serial.println("Server already connected");
  } if (!client.connected()) {
      Serial.println("No conecto");
  }

  // did that last thing work? sweet, let's do something
  if(client.connected()) {
    char homes[]="home";
    if (mqttclient.connect(homes)) {
      mqttclient.publish("home","home");
        Serial.println("Home connected");
    }
  } else {
    Serial.println("Cliente no conectado");
  }

  sendIP('I', octet);

}

//The main loop
void loop(void) {
  char message[10];

  // are we still connected?
  if (!client.connected()) {
    client = cc3000.connectTCP(server.ip, 1883); //En config.h cambiar la dirección IP del servidor suscriptor (averiguarv cual apara el linux o lo que sea)
    #ifdef DEBUG
      Serial.println("Server connected");
    #endif
    if(client.connected()) {
      if (mqttclient.connect("client")) {
        mqttclient.publish("home","home");
        mqttclient.publish("prueba","Funciona?");
          Serial.println("Home connected");
      }
    }
  } else {
    // Send sensor data
    sendSensor('A', sonar1);
    sendSensor('B', sonar2);
  }
  delay(DELAY);
}


//Send data sensor using a letter and the sonar
void sendSensor(char letra, NewPing sensor){
  int someInt = sensor.ping_cm();
    Serial.print("Dist ");
    Serial.print(letra);
    Serial.print(": ");
    Serial.println(someInt);
    char m[80];
    memset(&m[0],0,80);     //m[2]= 0;
    m[0]= letra;
    sprintf(&m[1], "%d", someInt);
    sendMessage("prueba",&m[0]); //Envía la palabra que hay en la dirección de memoria de m[0] hasta que encuentre un 0 en alguna dirección de memoria (en m[80] concretamente)
    
}


//Send IP to raspberry
void sendIP(char letra, unsigned char ip[4]){
  char mens[20];
  memset(&mens[0],0,20);

  //Calculo las longitudes de los números
  int long_ip[4] = {0,0,0,0};
  for(int i=0; i<4;i++){
    if (ip[i] > 100)
      long_ip[i] = 3;
    else if (ip[i] > 10)
      long_ip[i] = 2;
    else
      long_ip[i] = 1;
  }

  //Añado manualmente la IP
  mens[0] = letra;
  sprintf(&mens[0+1], "%d", ip[3]);
  mens[0+1+long_ip[3]] = '.';
  sprintf(&mens[0+1+long_ip[3]+1], "%d", ip[2]);
  mens[0+1+long_ip[3]+1+long_ip[2]] = '.';
  sprintf(&mens[0+1+long_ip[3]+1+long_ip[2]+1], "%d", ip[1]);
  mens[0+1+long_ip[3]+1+long_ip[2]+1+long_ip[1]] = '.';
  sprintf(&mens[0+1+long_ip[3]+1+long_ip[2]+1+long_ip[1]+1], "%d", ip[0]);

  sendMessage("prueba",&mens[0]);
}


/**
 * Send Message to the MQTT server
 * @param char* topic
 * @param char* message
 */
void sendMessage(char* topic, char* message)
{
  mqttclient.publish(topic, message);
}

void listSSIDResults(void){
  uint32_t index;
  uint8_t valid, rssi, sec;
  char ssidname[33];

  if (!cc3000.startSSIDscan(&index)) {
    Serial.println("No ha sido posible encontrar SSID!");
    return;
  }
  Serial.println("================================================");
  Serial.print("Redes encontradas: "); Serial.println(index);
  String S = "SSID" ;   S = S + " \t\t " + "RSSI" + " \t " + "Seguridad" ;
  Serial.println(S);
  Serial.println("================================================");
  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);

    Serial.print(ssidname);    Serial.print(" \t ");
    Serial.print(rssi);   Serial.print(" \t ");
    Serial.println(sec);
   }
  Serial.println("================================================");

  cc3000.stopSSIDscan();
}

bool displayConnectionDetails2(void){
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println("Esperando una direccion IP!\r\n");
    return false;
  }
  else
  {
    //Transfomo la IP de binario a partes
    for (int i=0; i<4;i++){
      octet[i] = (ipAddress >> (i*8) ) & 0xFF;
      prue[i] = (ipAddress >> (i*8) ) & 0xFF;
    }

    
    Serial.println("IP Addr: "); cc3000.printIPdotsRev(ipAddress);
    Serial.println("Netmask: "); cc3000.printIPdotsRev(netmask);
    Serial.println("Gateway: "); cc3000.printIPdotsRev(gateway);
    Serial.println("DHCPsrv: "); cc3000.printIPdotsRev(dhcpserv);
    Serial.println("DNSserv: "); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}
