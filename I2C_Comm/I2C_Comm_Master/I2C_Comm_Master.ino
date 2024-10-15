#include <Wire.h>

void setup() {
  Wire.begin();  // Join I2C bus as master
  Serial.begin(9600);  // Start serial communication
  delay(1000);  // Give some time for slave to initialize
}

void loop() {
  Wire.beginTransmission(8);  // Address of the slave device (8)
  Wire.write("Hey Slave");    // Send message to the slave
  Wire.endTransmission();     // End transmission

  delay(500);  // Wait for a response

  Wire.requestFrom(8, 11);    // Request 11 bytes from slave
  while (Wire.available()) {
    char c = Wire.read();     // Receive each byte
    Serial.print(c);          // Print response from slave
  }
  Serial.println();  // Newline for clarity

  delay(1000);  // Wait a bit before sending again
}
