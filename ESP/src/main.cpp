#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>

// Replace these with your Wi-Fi and Firebase credentials
#define WIFI_SSID "Y-3"
#define WIFI_PASSWORD "qazwsxed_C4"

#define FIREBASE_HOST "https://purifier-8101a-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Q58wJAPsvCC7SyfDzTWzzes4qSGd2wsO6XJtQEUM"

// Define Firebase objects
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
String path = "/sensorData";

void setup()
{
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Configure Firebase
  config.host = FIREBASE_HOST;
  config.api_key = FIREBASE_AUTH;
  auth.user.email = "your_email@example.com";
  auth.user.password = "your_password";

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  float sensorValue = analogRead(34); // Example sensor reading, using GPIO 34 (analog pin on ESP32)

  if (Firebase.pushFloat(firebaseData, path, sensorValue))
  {
    Serial.println("Data sent to Firebase successfully");
  }
  else
  {
    Serial.print("Failed to send data to Firebase: ");
    Serial.println(firebaseData.errorReason());
  }

  delay(2000); // Send data every 2 seconds
}