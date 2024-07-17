#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

EthernetUDP Udp; 
int powerPin1 = 2;
int directionPin1 = 3;

EthernetClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hi");
  initializeOSC();
  pinMode(powerPin1, OUTPUT);
  pinMode(directionPin1, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    int pin = Serial.parseInt();
    int state = Serial.parseInt();
    digitalWrite(pin, state);
  }
  
   OSCMessage msg;
   int size;

   if( (size = Udp.parsePacket())>0)
   {
     Serial.println("Message");
     while(size--){
       msg.fill(Udp.read());
     }

    //Serial.println(msg.hasError());
      if(!msg.hasError()){
        
        msg.route("/ping", pingback);
        msg.route("/enlight", enlight);
        msg.route("/setPin", setPinValue);
      }
   }
}
