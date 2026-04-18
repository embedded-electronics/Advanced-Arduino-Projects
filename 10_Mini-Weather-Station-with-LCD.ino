// Advanced Arduino Project
// Episode 10: Mini Weather Station with LCD

#include <LiquidCrystal.h>

// LCD Pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Sensor Pins
const int lm35Pin = A0;
const int humidSensorPin = A1;
const int windSensorPin = A2;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Weather Station");
  delay(2000);
  lcd.clear();
}

void loop() {

  // 🌡️ Temperature (LM35)
  int lm35Raw = analogRead(lm35Pin);
  float voltage = lm35Raw * (5.0 / 1023.0);
  float temperature = voltage * 100;   // LM35: 10mV per °C

  // 💧 Humidity (Simulated via soil sensor)
  int humidRaw = analogRead(humidSensorPin);
  int humidity = map(humidRaw, 0, 1023, 0, 100);

  // 🌬️ Wind Speed (Simulated via potentiometer)
  int windRaw = analogRead(windSensorPin);
  int windSpeed = map(windRaw, 0, 1023, 0, 100);

  // -------- LCD DISPLAY --------

  // Row 1 → Temperature
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature, 1);
  lcd.print("C   ");   // Clear extra chars

  // Row 2 → Humidity + Wind
  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("% ");

  lcd.print("W:");
  lcd.print(windSpeed);
  lcd.print("km ");

  delay(1000);
}
