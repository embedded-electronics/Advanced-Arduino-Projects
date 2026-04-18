// Advanced Arduino Project
// Episode 1: Smart Home Automation System

#include <math.h>

// ---------------- PIN DEFINITIONS ----------------
#define PIR_PIN 4         // PIR Motion Sensor
#define BUZZER_PIN 3      // Buzzer
#define BULB_PIN 2        // Bulb / LED
#define GAS_SENSOR A1     // Gas Sensor (MQ series)
#define THERMISTOR_PIN A0 // Thermistor
#define ULTRA_PIN 6       // Ultrasonic (Trig + Echo combined)
#define MOTOR_PIN 5       // Water Pump / Motor

// ---------------- THRESHOLDS ----------------
#define GAS_THRESHOLD 700
#define TEMP_THRESHOLD 35.0
#define WATER_EMPTY_DIST 20   // Tank Empty
#define WATER_FULL_DIST 5     // Tank Full

// ---------------- THERMISTOR CONSTANTS ----------------
const float SERIES_RESISTOR = 10000;
const float THERMISTOR_NOMINAL = 10000;
const float TEMPERATURE_NOMINAL = 25;
const float B_COEFFICIENT = 3950;

// ---------------- STATE VARIABLE ----------------
bool motorStatus = false;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BULB_PIN, OUTPUT);
  pinMode(ULTRA_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(BULB_PIN, LOW);
  digitalWrite(MOTOR_PIN, LOW);

  Serial.begin(9600);
  Serial.println("System Initializing...");
}

void loop() {

  bool alarmActive = false; // Flag for buzzer

  // -------- 1. MOTION DETECTION --------
  if (digitalRead(PIR_PIN) == HIGH) {
    digitalWrite(BULB_PIN, HIGH);   // Turn ON light
    alarmActive = true;
    Serial.println("Motion Detected → Bulb ON");
  } else {
    digitalWrite(BULB_PIN, LOW);
  }

  // -------- 2. GAS DETECTION --------
  int gasValue = analogRead(GAS_SENSOR);

  if (gasValue > GAS_THRESHOLD) {
    alarmActive = true;
    Serial.println("⚠ GAS LEAK DETECTED!");
    Serial.print("Gas Level: ");
    Serial.println(gasValue);
  }

  // -------- 3. TEMPERATURE DETECTION --------
  int analogValue = analogRead(THERMISTOR_PIN);

  float resistance = SERIES_RESISTOR / ((1023.0 / analogValue) - 1);
  float steinhart;

  steinhart = log(resistance / THERMISTOR_NOMINAL) / B_COEFFICIENT;
  steinhart += 1.0 / (TEMPERATURE_NOMINAL + 273.15);
  float tempC = (1.0 / steinhart) - 273.15;

  if (tempC > TEMP_THRESHOLD) {
    alarmActive = true;
    Serial.print("🔥 High Temperature: ");
    Serial.println(tempC);
  }

  // -------- 4. WATER LEVEL DETECTION --------
  float distance = getUltrasonicDistance();

  if (distance > WATER_EMPTY_DIST && distance < 400) {
    if (!motorStatus) {
      Serial.println("Water LOW → Motor ON");
    }
    motorStatus = true;
  }
  else if (distance < WATER_FULL_DIST) {
    if (motorStatus) {
      Serial.println("Water FULL → Motor OFF");
    }
    motorStatus = false;
  }

  digitalWrite(MOTOR_PIN, motorStatus);

  // -------- 5. BUZZER CONTROL --------
  digitalWrite(BUZZER_PIN, alarmActive);

  // -------- DEBUG OUTPUT --------
  Serial.print("Gas: "); Serial.print(gasValue);
  Serial.print(" | Temp: "); Serial.print(tempC);
  Serial.print(" | Water: "); Serial.println(distance);

  delay(500);
}

// -------- ULTRASONIC FUNCTION --------
float getUltrasonicDistance() {

  pinMode(ULTRA_PIN, OUTPUT);

  digitalWrite(ULTRA_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(ULTRA_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_PIN, LOW);

  pinMode(ULTRA_PIN, INPUT);

  long duration = pulseIn(ULTRA_PIN, HIGH, 25000);

  if (duration == 0) return 10; // Default value if no echo

  float distance = duration * 0.034 / 2; // Distance in cm

  return distance;
}
