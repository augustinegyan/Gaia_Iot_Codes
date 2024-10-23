#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LoRa.h>

const char *ssid = "TF4-2.4G"; // Replace with your WiFi name
const char *password = "kokomlelepoi"; // Replace with your WiFi password
const char *mqttServer = "broker.emqx.io"; // Replace with your MQTT broker address
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// LoRa settings
const int loRaCS = 15;
const int loRaReset = -1;
const int loRaIRQ = 5;

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  mqttClient.setServer(mqttServer, mqttPort);
  
  LoRa.begin(915E6);  // Initialize LoRa
  LoRa.setPins(loRaCS, loRaReset, loRaIRQ);
}

void loop() {
  // Check for incoming LoRa packets
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = LoRa.readString();
    Serial.println("Received: " + incoming);

    // Publish data to MQTT
    mqttClient.publish("sensor/data", incoming.c_str());
  }

  mqttClient.loop();  // Ensure MQTT client is working
}

// WiFi connection
void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}
