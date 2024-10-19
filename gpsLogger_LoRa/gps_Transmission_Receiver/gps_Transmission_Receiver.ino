#include <SPI.h>
#include <LoRa.h>

// Define pins for LoRa
#define SS 15
#define RST -1
#define DIO0 5

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);

  // Initialize LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(915E6)) {  // Use frequency 915 MHz for US, or 868 MHz for Europe
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa init succeeded.");
}

void loop() {
  // Check if there is incoming data
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Received a packet
    Serial.print("Received packet: ");
    
    // Read and print the packet content
    while (LoRa.available()) {
      String incoming = LoRa.readString();
      Serial.println(incoming);  // Print received GPS data (latitude,longitude)
    }
  }
}
