#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Define LoRa Pins
#define SS 15
#define RST -1
#define DIO0 5

// DHT22 sensor
#define DHTPIN 4  // GPIO2 (D4) for the data pin
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Wi-Fi credentials
const char* ssid = "Defense";
const char* password = "me1234567";

// MQTT broker details
const char* mqtt_server = "broker.hivemq.com";  // Public broker or your own
const int mqtt_port = 1883;
const char* mqtt_topic = "sensor/dht22";

// Create a Wi-Fi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect to Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

// Function to reconnect to the MQTT broker if disconnected
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);

  // Initialize LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(915E6)) {  // Set frequency (915 MHz for US, 868 MHz for Europe)
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa initialized!");

  // Initialize DHT22 sensor
  dht.begin();

  // Connect to Wi-Fi
  setup_wifi();

  // Initialize MQTT client
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // Reconnect to MQTT broker if disconnected
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Check for LoRa data packets
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String LoRaData = "";

    // Read the LoRa packet
    while (LoRa.available()) {
      LoRaData += (char)LoRa.read();
    }

    Serial.print("Received LoRa packet: ");
    Serial.println(LoRaData);

    // Publish the LoRa data to the MQTT broker
    if (client.publish(mqtt_topic, LoRaData.c_str())) {
      Serial.println("Data sent to MQTT broker");
    } else {
      Serial.println("MQTT publish failed");
    }
  }

  // Read data from DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any readings failed
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Format DHT22 data as a string
  String dhtData = "Temperature: " + String(t) + " °C  Humidity: " + String(h) + " %";
  Serial.println(dhtData);

  // Publish DHT22 data to MQTT broker
  if (client.publish(mqtt_topic, dhtData.c_str())) {
    Serial.println("DHT22 data sent to MQTT broker");
  } else {
    Serial.println("MQTT publish failed");
  }

  // Wait a few seconds before reading again
  delay(5000);
}
