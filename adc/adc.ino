#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads; // Create an instance of the ADS1115

void setup() {
    Serial.begin(9600); // Start serial communication
    ads.begin(); // Initialize the ADS1115
    ads.setGain(GAIN_ONE); // Set gain for the ADS1115
}

void loop() {
    int16_t adc0 = ads.readADC_SingleEnded(0); // Read from channel 0
    float voltage = adc0 * (4.096 / 32767.0); // Convert ADC value to voltage
    float temperatureC = voltage / 0.01; // Convert voltage to temperature in °C (10 mV/°C)

    // Debugging output
    Serial.print("ADC Value: ");
    Serial.print(adc0);
    Serial.print(", Voltage: ");
    Serial.print(voltage, 6); // Print voltage to 6 decimal places
    Serial.print(", Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");

    delay(1000); // Wait for 1 second before the next reading
}
