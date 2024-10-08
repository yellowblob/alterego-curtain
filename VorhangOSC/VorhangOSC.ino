#include <SPI.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

EthernetUDP Udp;
int powerPin1 = 2;
int directionPin1 = 3;
int powerPin2 = 4;
int directionPin2 = 5;

EthernetClient client;

IPAddress remoteIp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hi");
  initializeOSC();
  pinMode(powerPin1, OUTPUT);
  pinMode(directionPin1, OUTPUT);
  pinMode(powerPin2, OUTPUT);
  pinMode(directionPin2, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    int pin = Serial.parseInt();
    int state = Serial.parseInt();
    digitalWrite(pin, state);
  }

  OSCMessage msg;
  int size;

  if ((size = Udp.parsePacket()) > 0) {

    //store sender IP
    remoteIp = Udp.remoteIP();
    Serial.println(remoteIp);

    Serial.println("Message");
    while (size--) {
      msg.fill(Udp.read());
    }

    //Serial.println(msg.hasError());
    if (!msg.hasError()) {

      msg.route("/front/stop", frontStop);
      msg.route("/front/open", frontOpen);
      msg.route("/front/close", frontClose);
      msg.route("/back/stop", backStop);
      msg.route("/back/open", backOpen);
      msg.route("/back/close", backClose);
      msg.route("/ping", pingback);
      msg.route("/enlight", enlight);
      msg.route("/setPin", setPinValue);
    }
  }
}
