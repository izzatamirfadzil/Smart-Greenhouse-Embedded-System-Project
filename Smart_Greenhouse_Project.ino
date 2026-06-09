// Define the pins based on your schematic
const int soilMoisturePin = 34; 
const int pumpRelayPin = 26;    

// Define the moisture threshold for watering
const int dryThreshold = 30; 

void setup() {
  // Start the serial monitor at the standard ESP32 baud rate
  Serial.begin(115200); 
  
  // Initialize the pump relay pin as an output
  pinMode(pumpRelayPin, OUTPUT);
  
  // Ensure the pump is off initially
  digitalWrite(pumpRelayPin, LOW); 
}

void loop() {
  // 1. Read the raw analog value
  // The ESP32 has a 12-bit ADC, meaning it reads values from 0 to 4095.
  int rawMoistureValue = analogRead(soilMoisturePin);
  
  // 2. Convert the raw value to a percentage (0% to 100%)
  // Standard analog soil sensors usually output high values (e.g., 4095) when completely dry 
  // and lower values (e.g., 1000) when submerged in water. 
  int moisturePercentage = map(rawMoistureValue, 4095, 1000, 0, 100);
  
  // Constrain the percentage to stay strictly within 0-100
  moisturePercentage = constrain(moisturePercentage, 0, 100);

  // Print values to the Serial Monitor so you can verify the readings
  Serial.print("Raw Sensor Value: ");
  Serial.print(rawMoistureValue);
  Serial.print(" | Soil Moisture: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  // 3. Control the Water Pump based on the threshold
  if (moisturePercentage < dryThreshold) {
    Serial.println("Action: Soil is dry. Turning ON the water pump.");
    // Sending a HIGH signal to activate the relay
    digitalWrite(pumpRelayPin, HIGH); 
  } else {
    // Sending a LOW signal to deactivate the relay
    digitalWrite(pumpRelayPin, LOW);  
  }

  // Wait 2 seconds before taking the next reading
  delay(2000); 
}
