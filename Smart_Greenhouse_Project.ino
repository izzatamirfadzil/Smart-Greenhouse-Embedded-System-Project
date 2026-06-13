#include <DHT.h>

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

// Adjust after testing your LDR
int darkThreshold = 3000;

// Adjust after testing moisture sensor
int moistureThreshold = 2000;

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(115200);

  dht.begin();

  pinMode(FAN_RELAY_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(FAN_RELAY_PIN, LOW);
  digitalWrite(WATER_PUMP_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Smart Plant Monitoring System");
}

void loop() {

  // =====================
  // DHT11 Reading
  // =====================
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // =====================
  // LDR Reading
  // =====================
  int ldrValue = analogRead(LDR_PIN);

  // =====================
  // Moisture Reading
  // =====================
  int moistureValue = analogRead(MOISTURE_PIN);

  // =====================
  // Display Data
  // =====================
  Serial.println("---------------");

  if (!isnan(temperature) && !isnan(humidity)) {

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

  } else {
    Serial.println("DHT11 Read Failed");
  }

  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  Serial.print("Moisture Value: ");
  Serial.println(moistureValue);

  // =====================
  // Fan Control
  // =====================
  if (!isnan(temperature)) {

    if (temperature >= tempThreshold) {
      digitalWrite(FAN_RELAY_PIN, LOW);
      Serial.println("Fan: ON");
    } else {
      digitalWrite(FAN_RELAY_PIN, HIGH);
      Serial.println("Fan: OFF");
    }
  }

  // =====================
  // LED Control
  // =====================
  if (ldrValue > darkThreshold) {

    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED: ON (Dark)");

  } else {

    digitalWrite(LED_PIN, LOW);
    Serial.println("LED: OFF (Bright)");
  }

  // =====================
  // Water Pump Control (same logic style as fan)
  // =====================
  if (moistureValue >= moistureThreshold) {
    digitalWrite(WATER_PUMP_PIN, LOW);
    Serial.println("Pump: ON (Dry)");
  } else {
    digitalWrite(WATER_PUMP_PIN, HIGH);
    Serial.println("Pump: OFF (Wet)");
  }

  delay(2000);
}