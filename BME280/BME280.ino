#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Create an instance of the BME280 sensor
Adafruit_BME280 bme; 

void setup() {
  Serial.begin(9600); 
  // Check if BME280 sensor is found
  if (!bme.begin(0x76)) { // Some modules use address 0x76, others 0x77
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  // Read data from the BME280 sensor
  float temperature = bme.readTemperature();  // Temperature in Celsius
  float humidity = bme.readHumidity();        // Humidity in %
  float pressure = bme.readPressure() / 100.0F; // Pressure in hPa

  // Print the readings to the serial monitor
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  delay(2000); // Wait 2 seconds before taking another reading
}
