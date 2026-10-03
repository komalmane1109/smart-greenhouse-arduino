/*
  Smart Greenhouse Automation: integrated controller

  Combines the two sketches from the project report (see firmware/original_sketches/):
    - soil moisture  -> water pump
    - light (LDR)    -> grow light
    - temperature    -> cooling fan (via relay)
    - LCD shows temperature and humidity

  Changes from the original sketches:
    - The originals both used D3 and D4 (fan/light vs. DHT/relay), so they could not run
      together. Here the DHT sensor moves to D5; the fan stays on D3 and the light on D4.
    - Temperature for the fan now comes from the DHT sensor (30 C threshold, as in the
      LCD sketch) instead of the raw analog reading (threshold 200).
    - Loop delay is 2 s because the DHT22 should not be read faster than every 2 s.

  NOTE: sensor polarity differs between modules. Check on your hardware whether a LOW
  reading means "dry"/"dark" before relying on the comparisons below.

  Libraries (Arduino Library Manager): "DHT sensor library" and "Adafruit Unified Sensor"
  (Adafruit), and "LiquidCrystal I2C".
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ---------------- Pins ----------------
const int moisturePin = A0;   // soil moisture sensor (analog)
const int ldrPin      = A2;   // light dependent resistor (analog)
const int pumpPin     = 2;    // water pump (via relay)
const int fanPin      = 3;    // cooling fan (via relay)
const int lightPin    = 4;    // grow light strip (via relay)
#define DHTPIN 5              // DHT data pin
#define DHTTYPE DHT22         // report text says DHT11; simulation and sketches use DHT22

// ---------------- Thresholds ----------------
const int   dryThreshold       = 600;   // pump ON when moisture reading < this
const int   intensityThreshold = 600;   // light ON when LDR reading < this
const float tempThresholdC     = 30.0;  // fan ON when temperature > this

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // try 0x3F if the LCD stays blank

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(pumpPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  digitalWrite(pumpPin, LOW);
  digitalWrite(fanPin, LOW);
  digitalWrite(lightPin, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Smart Greenhouse");
  delay(2000);
  lcd.clear();
}

void loop() {
  int moistureLevel  = analogRead(moisturePin);
  int intensityLevel = analogRead(ldrPin);
  float temperature  = dht.readTemperature();  // Celsius
  float humidity     = dht.readHumidity();

  // Soil moisture -> pump
  digitalWrite(pumpPin, moistureLevel < dryThreshold ? HIGH : LOW);

  // Light -> grow light
  digitalWrite(lightPin, intensityLevel < intensityThreshold ? HIGH : LOW);

  Serial.print("Moisture: ");  Serial.print(moistureLevel);
  Serial.print("  Light: ");   Serial.print(intensityLevel);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("  DHT read failed");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    digitalWrite(fanPin, LOW);   // fail safe: fan off if temperature is unknown
    delay(2000);
    return;
  }

  // Temperature -> fan
  digitalWrite(fanPin, temperature > tempThresholdC ? HIGH : LOW);

  Serial.print("  Temp: ");      Serial.print(temperature);
  Serial.print(" C  Humidity: "); Serial.print(humidity);
  Serial.println(" %");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");

  delay(2000);
}
