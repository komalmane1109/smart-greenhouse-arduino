// Original sketch from the project report: DHT22 + I2C LCD + relay (simulated)
// (kept as written in the report for reference)
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 3        // DHT22 data pin connected to D3
#define DHTTYPE DHT22   // Using DHT22 sensor
#define RELAY_PIN 4     // Relay control pin
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust address if needed (0x3F or 0x27)

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Start with relay OFF

  lcd.setCursor(0, 0);
  lcd.print("DHT22 & Relay");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temperature = dht.readTemperature();  // Read temperature in Celsius
  float humidity = dht.readHumidity();        // Read humidity

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    return;
  }

  // Print to Serial Monitor
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("°C  Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");

  // Relay Control - Turn ON if temp > 30°C, OFF otherwise
  if (temperature > 30) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Relay ON");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Relay OFF");
  }

  delay(1000);  // Update every 1 seconds
}
