#include <SPI.h>
#include <LoRa.h>

// Define LoRa pins for ESP12F (ESP8266)
#define SS 15
#define RST -1  // Not used with ESP8266, set to -1
#define DIO0 5

void setup() {
  Serial.begin(9600);
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(915E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  
  LoRa.setSignalBandwidth(125E3);  // Optimize for lower bandwidth (125 kHz)
  LoRa.setSpreadingFactor(12);     // Max spreading factor for better range
  Serial.println("LoRa initialized.");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    // Received a packet
    String receivedMessage = LoRa.readString();
    Serial.println("Received message: " + receivedMessage);
    
    // Send ACK back to the sender
    LoRa.beginPacket();
    LoRa.print("ACK");
    LoRa.endPacket();
    Serial.println("ACK sent.");
  }
}
