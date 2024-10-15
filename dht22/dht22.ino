#include <DHT.h>

#define DHTPIN 2     // Pin connected to the DHT22 data pin
#define DHTTYPE DHT22   // DHT22 (AM2302) sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);  // Start serial communication
  dht.begin();         // Initialize the DHT22 sensor
}

void loop() {
  // Reading temperature and humidity takes about 250 milliseconds
  float humidity = dht.readHumidity();       // Read humidity
  float temperature = dht.readTemperature(); // Read temperature in Celsius

  // Check if any reads failed and exit early (to try again later)
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the results to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(2000);  // Wait 2 seconds between readings
}
