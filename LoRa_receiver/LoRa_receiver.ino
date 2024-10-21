#include <SPI.h>
#include <LoRa.h>

// Define LoRa pins
#define SS 15
#define RST -1
#define DIO0 5

void setup() {
  Serial.begin(9600);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(915E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  
  // Optimize for long range
  LoRa.setSpreadingFactor(12);  // Maximum range (SF12)
  LoRa.setSignalBandwidth(125E3);  // Lower bandwidth for longer range (125 kHz)
  
  Serial.println("LoRa initialized.");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Received a packet
    String message = LoRa.readString();
    Serial.println("Received message: " + message);
  }
}
