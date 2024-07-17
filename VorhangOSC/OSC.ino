
                              // A UDP instance to let us send and receive packets over UDP
byte mac[] = {
  0xA8, 0x61, 0x0A, 0xAE, 0x88, 0xFE
};
const IPAddress ip(192,168,178,150);   // IP of your board
  // IP of Control PC
const IPAddress eosIp(192,168,178,30);
const IPAddress ledPlayerIp(192,168,178,210);
const unsigned int outPort = 7000;          // remote port to receive OSC
const unsigned int localPort = 9000;        // local port to listen for OSC packets (actually not used for sending)
const unsigned int ledPlayerPort = 5000;  

void initializeOSC() {

    // Connect to WiFi network
    //Serial.println();
    //Serial.println();
    //WiFi.config(ip);
    //Serial.print("Connecting to ");
    //Serial.println(ssid);
    Ethernet.init(10); 
    Ethernet.begin(mac, ip);

   // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start UDP
  Udp.begin(localPort);
}

void sendOSC(char address[], int value) {
    OSCMessage msg(address);
    msg.add(value);
    Udp.beginPacket(eosIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
}

void sendUDP(char message[]) {
    Udp.beginPacket(ledPlayerIp, ledPlayerPort);
    Udp.write(message);
    Udp.endPacket();
}

void enlight(OSCMessage &msg){
  if (msg.isInt(0)){
    Serial.print("received package: ");
    int val = msg.getInt(0);
    Serial.println(val);
    digitalWrite(LED_BUILTIN, val);  
  }
}

void setPinValue(OSCMessage &msg){
  Serial.print("received package: ");
  if (msg.isInt(0)){
    int pin = msg.getInt(0);
    Serial.print(pin);
    Serial.print(" ");
    if (msg.isInt(1)){
      int state = msg.isInt(1);
      Serial.println(state);
      digitalWrite(pin, state);
    } 
  }
}

void pingback(OSCMessage &msg){
   Serial.print("received package: ");
   OSCMessage outMsg("/curtain/ping");
   outMsg.add(true);
   Udp.beginPacket(eosIp, outPort);
   outMsg.send(Udp); // send the bytes to the SLIP stream
   Udp.endPacket(); // mark the end of the OSC Packet
   outMsg.empty(); // free space occupied by message
}
