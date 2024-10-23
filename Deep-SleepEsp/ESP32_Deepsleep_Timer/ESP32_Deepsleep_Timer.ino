#include "esp_sleep.h"

#define WAKEUP_PIN GPIO_NUM_33  // GPIO pin connected to the 4-pin switch

void setup() {
  Serial.begin(115200);
  delay(1000);  // Delay to allow time for serial monitor

  // Check the wake-up reason
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Woke up from GPIO!");
  } else {
    Serial.println("Setting up for deep sleep. Press the button to wake up.");
  }

  // Enable internal pull-up resistor on GPIO 33
  pinMode(WAKEUP_PIN, INPUT_PULLUP);

  // Configure wake-up source: GPIO 33 with LOW signal
  esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, 0);  // 0 = LOW level wake-up

  // Enter deep sleep
  Serial.println("Entering deep sleep now...");
  esp_deep_sleep_start();
}

void loop() {
  // Empty loop as the ESP32 will sleep in setup
}
