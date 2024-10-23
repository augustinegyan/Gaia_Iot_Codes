#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI";  // Your WiFi network name (SSID)
const char* password = "YOUR_WIFI_PASSWORD";  // Your WiFi password

// MQTT Broker settings
const char* mqtt_server = "broker.emqx.io";  // MQTT broker address
const int mqtt_port = 1883;  // MQTT broker port (default unencrypted port)
const char* mqtt_topic_pub = "mq7/sensor/data";  // Topic to publish messages to

// Custom client ID (must be unique for each device)
const char* clientID = "MyMQ7Client";  // Change this to ensure uniqueness

WiFiClient espClient;  // Create a WiFi client object
PubSubClient client(espClient);  // Create a PubSubClient object using the WiFi client

// Pin for MQ7 sensor
const int mq7_pin = A0;  // MQ7 connected to analog pin A0

// Function to connect to WiFi
void setup_wifi() {
  delay(10);  // Short delay for stability
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);  // Initiate WiFi connection
  while (WiFi.status() != WL_CONNECTED) {  // Wait until WiFi is connected
    delay(500);
    Serial.print(".");  // Print dots while connecting
  }
  Serial.println("WiFi connected");  // Print message when connected
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  // Print the local IP address
}

// Function to reconnect to the MQTT broker
void reconnect() {
  while (!client.connected()) {  // Loop until we're reconnected
    Serial.print("Attempting MQTT connection...");
    // Connect with just the client ID
    if (client.connect(clientID)) {  // Connect with client ID only
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());  // Print the reason for connection failure
      Serial.println(" trying again in 5 seconds");
      delay(5000);  // Wait 5 seconds before retrying
    }
  }
}

// Setup function - runs once when the device starts or resets
void setup() {
  Serial.begin(115200);  // Initialize serial communication at 115200 baud rate
  setup_wifi();  // Call function to connect to WiFi
  client.setServer(mqtt_server, mqtt_port);  // Set MQTT server and port
}

// Function to send MQ7 sensor data to MQTT broker
void sendSensorData() {
  // Read MQ7 sensor value
  int mq7_value = analogRead(mq7_pin);

  // Prepare payload
  String payload = "MQ7 sensor value: " + String(mq7_value);

  // Publish sensor data to the MQTT topic
  client.publish(mqtt_topic_pub, payload.c_str());

  // Log the published message
  Serial.println("Published: " + payload);
}

// Main loop function - runs repeatedly after setup
void loop() {
  if (!client.connected()) {
    reconnect();  // Reconnect to MQTT broker if connection is lost
  }
  client.loop();  // Maintain MQTT connection and process incoming messages

  // Send sensor data to the MQTT broker every 10 seconds
  static unsigned long lastSend = 0;
  unsigned long now = millis();
  if (now - lastSend > 10000) {  // 10-second interval
    sendSensorData();  // Send MQ7 sensor data
    lastSend = now;
  }
}
