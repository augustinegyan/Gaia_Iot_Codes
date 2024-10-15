#include <Wire.h>

void setup() {
  Wire.begin(8);  // Join I2C bus with address 8
  Wire.onReceive(receiveEvent);  // Register function for receiving data
  Wire.onRequest(requestEvent);  // Register function for sending data
  Serial.begin(9600);  // Start serial communication
}

void loop() {
  delay(100);  // Allow the events to process
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();    // Read each byte from master
    Serial.print(c);         // Print received message
  }
  Serial.println();  // Newline for clarity
}

void requestEvent() {
  Wire.write("Hello Master");  // Send response back to master
}
