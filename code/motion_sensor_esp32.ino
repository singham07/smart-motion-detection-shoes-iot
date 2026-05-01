#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// WiFi Credentials
const char* ssid = "YOUR_WIFI_PASSWORD";
const char* password = "YOUR_PASSWORD";

// ThingWorx Config
const char* server = "http://172.16.78.20:7080/Thingworx/Things/MOTION_DETECTION/Properties/*";
const char* appKey = "5cbc25ff-5b80-4f43-ac4f-edf6687e08b9";

// Sensor Object
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
  Serial.begin(115200);

  // I2C init
  Wire.begin(21, 22);

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  // Sensor init
  if (!accel.begin()) {
    Serial.println("❌ ADXL345 not detected!");
    while (1);
  }

  Serial.println("✅ Sensor Ready");
}

void loop() {

  // Read sensor
  sensors_event_t event;
  accel.getEvent(&event);

  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;

  Serial.printf("X: %.2f | Y: %.2f | Z: %.2f\n", x, y, z);

  // Send to ThingWorx
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(server);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("appKey", appKey);

    // JSON payload
    String payload = "{";
    payload += "\"x_value\":" + String(x, 2) + ",";
    payload += "\"y_value\":" + String(y, 2) + ",";
    payload += "\"z_value\":" + String(z, 2);
    payload += "}";

    int response = http.PUT(payload);

    Serial.print("HTTP Response: ");
    Serial.println(response);

    http.end();
  } else {
    Serial.println("⚠️ WiFi Disconnected");
  }

  delay(2000);
}