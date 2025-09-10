#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-Fi and MQTT Broker settings
const char* ssid = "iitk";            // Replace with your Wi-Fi SSID
const char* password = "";     // Replace with your Wi-Fi password
const char* mqtt_server = "172.26.213.93"; // Replace with your MQTT broker IP
const int mqtt_port = 1883;                  // Replace with your MQTT broker port

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
  
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to reconnect to the MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Check if data is available from the Arduino
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n'); // Read until newline character
    Serial.print("Sending message: ");
    Serial.println(message);

    // Publish the message to an MQTT topic
    client.publish("CE738/Project3/Group5/data", message.c_str());
  }
}
