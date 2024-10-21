#include <SPI.h>
#include <LoRa.h>

// LoRa pin definitions
#define SS 15         // Chip select pin
#define RST -1        // Reset pin (set to -1 if not used)
#define DIO0 5        // IRQ pin

// LoRa frequency (same as sender)
#define LORA_FREQUENCY 915E6

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  while (!Serial);

  // Initialize LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa init succeeded.");
}

void loop() {
  // Check for incoming LoRa packets
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Print incoming message to Serial
    Serial.print("Received packet: ");
    String incoming = LoRa.readString();
    Serial.println(incoming);
    
    // Send back an ACK
    LoRa.beginPacket();
    LoRa.print("ACK"); // Acknowledge the received message
    LoRa.endPacket();
    Serial.println("ACK sent back.");
  }
}
