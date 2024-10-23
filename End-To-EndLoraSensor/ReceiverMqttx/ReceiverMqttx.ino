#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// LoRa pin definitions
#define SS 15        // Chip select pin
#define RST -1       // Reset pin (set to -1 if not used)
#define DIO0 5       // IRQ pin

// LoRa frequency (same as sender)
#define LORA_FREQUENCY 915E6

// Wi-Fi credentials
const char* ssid = "";
const char* password = "";

// MQTT broker details
const char* mqtt_server = "broker.emqx.io"; // Example: change to your MQTT broker
const int mqtt_port = 1883;
const char* mqtt_topic = "lora/data";

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Function declarations
void reconnect();
void enterDeepSleep();

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  while (!Serial);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Set up MQTT
  client.setServer(mqtt_server, mqtt_port);

  // Initialize LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  Serial.println("LoRa init succeeded.");
}

void loop() {
  // Reconnect to MQTT if necessary
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Check for incoming LoRa packets
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Print incoming message to Serial
    Serial.print("Received packet: ");
    String incoming = LoRa.readString();
    Serial.println(incoming);

    // Send data to MQTT broker
    if (client.publish(mqtt_topic, incoming.c_str())) {
      Serial.println("Data sent to MQTT broker successfully.");
    } else {
      Serial.println("Failed to send data to MQTT broker.");
    }

    // Send back an ACK
    LoRa.beginPacket();
    LoRa.print("ACK"); // Acknowledge the received message
    LoRa.endPacket();
    Serial.println("ACK sent back.");

    // Enter deep sleep to conserve power
    Serial.println("Entering deep sleep for 10 seconds...");
    enterDeepSleep();
  }
}

// Reconnect to the MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a client ID based on the chip ID
    String clientId = "ESP8266Client-";
    clientId += String(ESP.getChipId());

    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, subscribe if needed
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Enter deep sleep for power saving
void enterDeepSleep() {
  // Sleep for 10 seconds (time is in microseconds)
  ESP.deepSleep(10e6);  // 10 seconds
}
