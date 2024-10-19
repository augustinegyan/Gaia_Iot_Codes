#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>  // Include the SoftwareSerial library
#include <SPI.h>
#include <LoRa.h>

// Define pins for LoRa
#define SS 10
#define RST 9
#define DIO0 7

// Define pins for SoftwareSerial (for GPS communication)
#define RX_PIN 20   // Connect to GPS TX
#define TX_PIN 21   // Connect to GPS RX

SoftwareSerial gpsSerial(RX_PIN, TX_PIN);  // Create a SoftwareSerial object

// Initialize GPS
TinyGPSPlus gps;

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);

  // Initialize software serial for GPS
  gpsSerial.begin(9600);

  // Initialize LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(915E6)) {  // Use frequency 915 MHz for US, or 868 MHz for Europe
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa init succeeded.");
}

void loop() {
  // Check if there's GPS data available
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    // Get GPS data
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();
    String gpsData = String(latitude, 6) + "," + String(longitude, 6);

    // Send GPS data via LoRa
    LoRa.beginPacket();
    LoRa.print(gpsData);
    LoRa.endPacket();
    
    // Print GPS data to Serial Monitor for debugging
    Serial.print("Sent: ");
    Serial.println(gpsData);
  }

  // Wait before sending the next data
  delay(2000);
}
