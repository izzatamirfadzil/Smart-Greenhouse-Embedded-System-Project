#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// =====================
// WiFi Credentials
// =====================
const char* ssid = "wifi sejati_5G";
const char* password = "geng1234";

// =====================
// ThingsBoard
// =====================
const char* server = "http://demo.thingsboard.io/api/v1/MKyTiePKBgMxXSAnJTOW/telemetry";

// =====================
// Pin Definitions
// =====================
#define DHTPIN 4
#define DHTTYPE DHT11

#define FAN_RELAY_PIN 26
#define WATER_PUMP_PIN 27

#define LDR_PIN 35
#define LED_PIN 2

#define MOISTURE_PIN 32

// =====================
// Settings
// =====================
float tempThreshold = 35.0;
int darkThreshold = 3000;
int moistureThreshold = 2000;

DHT dht(DHTPIN, DHTTYPE);

// =====================
// WiFi Setup
// =====================
void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
}

void setup() {

  Serial.begin(115200);

  connectWiFi();

  dht.begin();

  pinMode(FAN_RELAY_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(FAN_RELAY_PIN, HIGH);
  digitalWrite(WATER_PUMP_PIN, HIGH);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Smart Plant Monitoring System");
}

void loop() {

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  int ldrValue = analogRead(LDR_PIN);
  int moistureValue = analogRead(MOISTURE_PIN);

  Serial.println("---------------");

  // =====================
  // DHT
  // =====================
  if (!isnan(temperature) && !isnan(humidity)) {

    Serial.print("Temp: ");
    Serial.println(temperature);

    Serial.print("Humidity: ");
    Serial.println(humidity);
  }

  // =====================
  // LDR
  // =====================
  Serial.print("LDR: ");
  Serial.println(ldrValue);

  // =====================
  // Moisture
  // =====================
  Serial.print("Moisture: ");
  Serial.println(moistureValue);

  // =====================
  // FAN LOGIC (UNCHANGED)
  // =====================
  int fanState = 0;

  if (temperature >= tempThreshold) {
    digitalWrite(FAN_RELAY_PIN, LOW);
    Serial.println("Fan: ON");
    fanState = 1;
  } else {
    digitalWrite(FAN_RELAY_PIN, HIGH);
    Serial.println("Fan: OFF");
  }

  // =====================
  // LED LOGIC (UNCHANGED)
  // =====================
  if (ldrValue > darkThreshold) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED: ON (Dark)");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED: OFF (Bright)");
  }

  // =====================
  // PUMP LOGIC (UNCHANGED)
  // =====================
  int pumpState = 0;

  if (moistureValue >= moistureThreshold) {
    digitalWrite(WATER_PUMP_PIN, LOW);
    Serial.println("Pump: ON (Dry)");
    pumpState = 1;
  } else {
    digitalWrite(WATER_PUMP_PIN, HIGH);
    Serial.println("Pump: OFF (Wet)");
  }

  // =====================
  // SEND TO THINGSBOARD
  // =====================
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    http.begin(server);
    http.addHeader("Content-Type", "application/json");

    String payload = "{";
    payload += "\"temperature\":" + String(temperature) + ",";
    payload += "\"humidity\":" + String(humidity) + ",";
    payload += "\"ldr\":" + String(ldrValue) + ",";
    payload += "\"moisture\":" + String(moistureValue) + ",";
    payload += "\"fan\":" + String(fanState) + ",";
    payload += "\"pump\":" + String(pumpState);
    payload += "}";

    int httpResponseCode = http.POST(payload);

    Serial.print("ThingsBoard Response: ");
    Serial.println(httpResponseCode);

    http.end();
  }

  delay(2000);
}