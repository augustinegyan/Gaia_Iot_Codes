#include <SPI.h>
#include <LoRa.h>

// Define LoRa pins (modify according to your setup)
#define SS 15      // LoRa Chip Select
#define RST -1     // LoRa Reset (can be ignored if not used)
#define DIO0 5     // LoRa IRQ (Interrupt Request Pin)

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
      Serial.println(incoming);  // Print received message
    }
  }
}
