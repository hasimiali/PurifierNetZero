#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>

// Replace these with your Wi-Fi and Firebase credentials
#define WIFI_SSID "Y-3"
#define WIFI_PASSWORD "qazwsxed_C4"

#define FIREBASE_HOST "https://purifier-8101a-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Q58wJAPsvCC7SyfDzTWzzes4qSGd2wsO6XJtQEUM"

// Firebase database paths
#define DATABASE_PATH "/sensors"

// Define sensor pins
#define SENSOR1_PIN 34
#define SENSOR2_PIN 35
#define SENSOR3_PIN 32
#define SENSOR4_PIN 33
#define SENSOR5_PIN 25

FirebaseData firebaseData;
FirebaseJson json;
FirebaseConfig config;
FirebaseAuth auth;

// Function to read sensor data
int readSensor(int pin)
{
  return analogRead(pin);
}

void setup()
{
  Serial.begin(115200);

  // Initialize WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to Wi-Fi with IP: ");
  Serial.println(WiFi.localIP());

  // Set up Firebase config
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Initialize sensors (if any additional setup is needed)
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  pinMode(SENSOR3_PIN, INPUT);
  pinMode(SENSOR4_PIN, INPUT);
  pinMode(SENSOR5_PIN, INPUT);
}

void loop()
{
  // Read data from sensors
  int sensor1Data = readSensor(SENSOR1_PIN);
  int sensor2Data = readSensor(SENSOR2_PIN);
  int sensor3Data = readSensor(SENSOR3_PIN);
  int sensor4Data = readSensor(SENSOR4_PIN);
  int sensor5Data = readSensor(SENSOR5_PIN);

  // Create JSON object
  json.clear();
  json.add("sensor1", sensor1Data);
  json.add("sensor2", sensor2Data);
  json.add("sensor3", sensor3Data);
  json.add("sensor4", sensor4Data);
  json.add("sensor5", sensor5Data);

  // Print JSON to Serial
  String jsonStr;
  json.toString(jsonStr, true);
  Serial.println(jsonStr);

  // Send JSON data to Firebase
  if (Firebase.setJSON(firebaseData, DATABASE_PATH, json))
  {
    Serial.println("Data sent successfully");
  }
  else
  {
    Serial.print("Error sending data: ");
    Serial.println(firebaseData.errorReason());
  }

  // Wait for 10 seconds before sending the next batch of data
  delay(10000);
}