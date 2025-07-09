#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "Secrets.h"

ESP8266WiFiMulti wifiMulti;

// IP Config
IPAddress staticIP(192, 168, 1, 250);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);

// AWS
WiFiClientSecure awsClient;
BearSSL::X509List cert(CA_CERT);
BearSSL::X509List client_crt(CLIENT_CERT);
BearSSL::PrivateKey key(PRIVATE_KEY);
PubSubClient client(awsClient);

// Pins
const uint8_t ANALOG_PIN = A0;

void connectToWiFi() {
  Serial.println('\n');

  if (!WiFi.config(staticIP, gateway, subnet, primaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  wifiMulti.addAP(SSID_1, PASSWORD);
  wifiMulti.addAP(SSID_2, PASSWORD);

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(' ');
    Serial.print(++i);
    delay(1000);
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}

void connectToNTP() {
  configTime(TIMEZONE * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

char* getCurrentTime() {
  static char formattedTime[20];

  time_t now = time(nullptr);  // Get current epoch time
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);  // Convert to local time (with timezone)

  strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M", &timeinfo);

  return formattedTime;
}

void connectToAWS() {
  awsClient.setTrustAnchors(&cert);
  awsClient.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_HOST, 8883);

  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THING_NAME)) {
    Serial.print(".");
    delay(1000);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishMessageToAWS() {
  JsonDocument doc;
  doc["date"] = getCurrentTime();
  doc["humidity"] = analogRead(ANALOG_PIN);
  doc["plant"] = PLANT_NAME;
  doc["location"] = PLANT_LOCATION;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer, sizeof(jsonBuffer));  // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(ANALOG_PIN, INPUT);

  connectToWiFi();
  connectToNTP();
  connectToAWS();
}

void loop() {
  // publishMessageToAWS();
  Serial.println(analogRead(ANALOG_PIN));
  delay(500);
}
