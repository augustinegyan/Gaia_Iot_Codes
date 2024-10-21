#include <SPI.h>
#include <LoRa.h>
#include <LowPower.h>  // Library for ATmega328 power-saving

// Define LoRa pins for ATmega328
#define SS 10
#define RST 9
#define DIO0 7

#define MAX_RETRIES 3  // Max number of retries
#define ACK_TIMEOUT 2000  // 2 seconds timeout for receiving ACK

void setup() {
  Serial.begin(9600);
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(915E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  
  LoRa.setSignalBandwidth(125E3);  // Optimize for lower bandwidth (125 kHz)
  LoRa.setSpreadingFactor(12);     // Max spreading factor for better range
  Serial.println("LoRa initialized.");
}

void loop() {
  int retries = 0;
  bool ackReceived = false;

  while (retries < MAX_RETRIES && !ackReceived) {
    // Send message
    LoRa.beginPacket();
    LoRa.print("Hello Receiver");
    LoRa.endPacket();
    Serial.print("Message sent, attempt ");
    Serial.println(retries + 1);

    // Wait for ACK
    long startTime = millis();
    while (millis() - startTime < ACK_TIMEOUT) {
      int packetSize = LoRa.parsePacket();
      if (packetSize) {
        String receivedMessage = LoRa.readString();
        if (receivedMessage == "ACK") {
          ackReceived = true;
          Serial.println("ACK received!");
          break;
        }
      }
    }

    if (!ackReceived) {
      retries++;
      Serial.println("No ACK, retrying...");
      delay(1000);  // Wait 1 second before retrying
    }
  }

  if (!ackReceived) {
    Serial.println("Max retries reached, giving up.");
  }

  // Power down to save energy (sleep for 8 seconds)
  Serial.println("Entering sleep mode...");
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

  delay(5000);  // Delay before the next message is sent
}
