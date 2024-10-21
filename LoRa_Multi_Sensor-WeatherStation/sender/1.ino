#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

// Pin definitions for LoRa and sensors
#define SS 10         // LoRa chip select pin
#define RST  9       // LoRa reset pin, set to -1 if not used
#define DIO0 7        // LoRa IRQ pin
#define DHTPIN 2      // DHT sensor data pin
#define DHTTYPE DHT22 // DHT22 or DHT11, choose your sensor model
#define MQ7PIN A0     // MQ7 analog pin

// LoRa frequency (915 MHz for US, 868 MHz for Europe)
#define LORA_FREQUENCY 915E6

// Create instances for the sensors
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BME280 bme;

// Timer for sending data periodically
unsigned long lastSendTime = 0;
const unsigned long interval = 10000; // 10 seconds

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  while (!Serial);

  // Initialize DHT sensor
  dht.begin();

  // Initialize BME280 sensor
  if (!bme.begin(0x76)) { // Check if BME280 is at address 0x76
    Serial.println("Could not find BME280 sensor!");
    while (1);
  }

  // Initialize LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa init succeeded.");
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastSendTime > interval) {
    // Read data from the DHT sensor
    float dhtHumidity = dht.readHumidity();
    float dhtTemp = dht.readTemperature();

    // Read data from the BME280 sensor
    float bmeTemp = bme.readTemperature();
    float bmePressure = bme.readPressure() / 100.0F; // Convert to hPa
    float bmeHumidity = bme.readHumidity();

    // Read data from MQ7 sensor (for CO levels)
    int mq7Value = analogRead(MQ7PIN);

    // Check if any readings failed and exit early (to try again)
    if (isnan(dhtHumidity) || isnan(dhtTemp) || isnan(bmeTemp) || isnan(bmePressure) || isnan(bmeHumidity)) {
      Serial.println("Failed to read from sensors!");
      return;
    }

    // Print the readings to Serial (for debugging)
    Serial.println("Sending data via LoRa...");
    
    // Prepare the data string to send
    String dataToSend = "DHT Temp: " + String(dhtTemp) + "C, " +
                        "DHT Humidity: " + String(dhtHumidity) + "%, " +
                        "BME Temp: " + String(bmeTemp) + "C, " +
                        "BME Pressure: " + String(bmePressure) + "hPa, " +
                        "BME Humidity: " + String(bmeHumidity) + "%, " +
                        "MQ7 CO Level: " + String(mq7Value);
    
    // Send the data via LoRa
    LoRa.beginPacket();
    LoRa.print(dataToSend);
    LoRa.endPacket();

    // Wait for an ACK from the receiver
    Serial.println("Waiting for ACK...");
    unsigned long ackStartTime = millis();
    bool ackReceived = false;

    // Check for ACK within a timeout period
    while (millis() - ackStartTime < 5000) { // 5 seconds timeout
      int packetSize = LoRa.parsePacket();
      if (packetSize) {
        String incoming = LoRa.readString();
        if (incoming == "ACK") { // Check for ACK message
          ackReceived = true;
          break;
        }
      }
    }

    if (ackReceived) {
      Serial.println("ACK received!");
    } else {
      Serial.println("No ACK received.");
    }

    lastSendTime = currentMillis;
  }
}
