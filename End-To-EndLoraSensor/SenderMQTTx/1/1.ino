#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Sensor setup
Adafruit_BME280 bme;  // I2C
const int mq7Pin = A0;  // MQ7 sensor connected to analog pin

// LoRa settings
const int loRaCS = 10;
const int loRaReset = 9;
const int loRaIRQ = 7;

void setup() {
  Serial.begin(115200);
  LoRa.begin(915E6);  // Set the frequency to match your LoRa module
  bme.begin(0x76);  // Default I2C address of BME280

  LoRa.setPins(loRaCS, loRaReset, loRaIRQ);
}

void loop() {
  // Read sensor data
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F; // Convert to hPa
  int mq7Value = analogRead(mq7Pin);

  // Prepare payload
  String payload = String(temperature) + "," + String(humidity) + "," + String(pressure) + "," + String(mq7Value);

  // Send data via LoRa
  LoRa.beginPacket();
  LoRa.print(payload);
  LoRa.endPacket();

  delay(10000);  // Send every 10 seconds
}
