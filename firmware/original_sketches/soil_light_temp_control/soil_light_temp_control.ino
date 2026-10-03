// Original sketch from the project report: soil moisture, temperature and light control
// (kept as written in the report for reference)

// Pin Definitions
const int moisturePin = A0;  // Analog pin connected to the soil moisture sensor
const int pump = 2;          // Pin connected to the water pump
const int temppin = A1;      // Analog pin for temperature sensor
const int fan = 3;           // Pin connected to the cooling fan
const int ldrpin = A2;       // Analog pin for light intensity sensor
const int light = 4;         // Pin connected to the light source

// Thresholds
const int dryThreshold = 600;       // Moisture level threshold
const int tempThreshold = 200;      // Temperature threshold
const int intensityThreshold = 600; // Light intensity threshold

void setup() {
  Serial.begin(9600);

  // Set output pins
  pinMode(pump, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(light, OUTPUT);
}

void loop() {
  // Read sensor values
  int moistureLevel = analogRead(moisturePin);
  int tempLevel = analogRead(temppin);
  int intensityLevel = analogRead(ldrpin);

  // Print values to Serial Monitor
  Serial.print("Moisture Level: ");
  Serial.println(moistureLevel);
  Serial.print("Temperature Level: ");
  Serial.println(tempLevel);
  Serial.print("Intensity Level: ");
  Serial.println(intensityLevel);

  // Control water pump
  if (moistureLevel < dryThreshold) {
    digitalWrite(pump, HIGH);
  } else {
    digitalWrite(pump, LOW);
  }

  // Control cooling fan
  if (tempLevel > tempThreshold) {
    digitalWrite(fan, HIGH);
  } else {
    digitalWrite(fan, LOW);
  }

  // Control light
  if (intensityLevel < intensityThreshold) {
    digitalWrite(light, HIGH);
  } else {
    digitalWrite(light, LOW);
  }

  delay(1000);  // Update every second
}
