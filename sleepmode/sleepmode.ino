#include <LowPower.h>

void setup() {
  // Set up Serial for debugging (optional)
  Serial.begin(9600);

  // Wait for Serial monitor to open
  while (!Serial);

  // Initial setup, e.g., turning on an LED to show active state
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Entering sleep mode...");
}

void loop() {
  // Turn on the LED before going to sleep
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Microcontroller is awake!");
  
  // Wait for a second to visualize that the LED is on
  delay(1000);

  // Turn off the LED to indicate sleep mode
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Microcontroller is going to sleep...");

  // Sleep for 5 seconds (5000 milliseconds)
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); // 1-second sleep
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); // Repeat for 5 times
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);

  // After 5 seconds of sleep, the microcontroller will wake up and continue
  Serial.println("Microcontroller woke up!");
}
