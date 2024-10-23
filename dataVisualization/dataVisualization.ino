#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "";           // Replace with your SSID
const char* password = "";   // Replace with your password

// MQTT broker details
const char* mqtt_server = "broker.emqx.io"; // Replace with your MQTT broker
const char* mqtt_topic = "sensor/mq7";        // Topic to publish the MQ7 data

// Create instances for WiFi and MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// MQ7 Sensor Pin
const int mq7Pin = A0;

void setup() {
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Set MQTT server
  client.setServer(mqtt_server, 1883);
}

void loop() {
  // Reconnect to MQTT broker if disconnected
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read MQ7 sensor value
  int mq7Value = analogRead(mq7Pin);
  Serial.print("MQ7 Sensor Value: ");
  Serial.println(mq7Value);

  // Publish MQ7 data
  String payload = String(mq7Value);
  client.publish(mqtt_topic, payload.c_str());

  // Wait before the next reading
  delay(2000); // Adjust the delay as needed
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe or publish as needed
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
