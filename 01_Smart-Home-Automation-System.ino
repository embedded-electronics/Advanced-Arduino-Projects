#include <math.h>
 
// Pin Definitions
#define PIR_PIN 4
#define BUZZER_PIN 3
#define BULB_PIN 2
#define GAS_SENSOR A1
#define THERMISTOR_PIN A0
#define ULTRA_PIN 6 
#define MOTOR_PIN 5
 
// Constants
#define GAS_THRESHOLD 700       // Set to 700 as requested
#define TEMP_THRESHOLD 35.0     
#define WATER_EMPTY_DIST 20     // Higher distance = water level is LOW
#define WATER_FULL_DIST 5       // Lower distance = water level is HIGH
 
// Thermistor constants
const float SERIES_RESISTOR = 10000;
const float THERMISTOR_NOMINAL = 10000;
const float TEMPERATURE_NOMINAL = 25;
const float B_COEFFICIENT = 3950;
 
// State Variables
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
  bool alarmActive = false; // Buzzer flag
 
  // --- 1. Motion Detection ---
  if (digitalRead(PIR_PIN) == HIGH) {
    digitalWrite(BULB_PIN, HIGH);
    alarmActive = true; 
    Serial.println("-> Motion Detected! Bulb ON");
  } else {
    digitalWrite(BULB_PIN, LOW);
  }
 
  // --- 2. Gas Detection ---
  int gasValue = analogRead(GAS_SENSOR);
  if (gasValue > GAS_THRESHOLD) {
    alarmActive = true;
    Serial.println("GAS LEAKAGE DETECTED!"); // Added specific print
    Serial.print("Current Gas Level: "); Serial.println(gasValue);
  }
 
  // --- 3. Temperature Detection ---
  int analogValue = analogRead(THERMISTOR_PIN);
  float resistance = SERIES_RESISTOR / ((1023.0 / analogValue) - 1);
  float steinhart = log(resistance / THERMISTOR_NOMINAL) / B_COEFFICIENT;
  steinhart += 1.0 / (TEMPERATURE_NOMINAL + 273.15);
  float tempC = (1.0 / steinhart) - 273.15;
  
  if (tempC > TEMP_THRESHOLD) {
    alarmActive = true;
    Serial.print("High Temp Alert: "); Serial.println(tempC);
  }
 
  // --- 4. Water Level Detection (Hysteresis) ---
  float distance = getUltrasonicDistance();
 
  if (distance > WATER_EMPTY_DIST && distance < 400) {
    if (motorStatus == false) { // Only print when state changes
       Serial.println("WATER EMPTY: MOTOR ON");
    }
    motorStatus = true;
  } 
  else if (distance < WATER_FULL_DIST) {
    if (motorStatus == true) { // Only print when state changes
       Serial.println("WATER FULL: MOTOR OFF");
    }
    motorStatus = false;
  }
 
  digitalWrite(MOTOR_PIN, motorStatus ? HIGH : LOW);
 
  // --- Final Buzzer Control (Unchanged Logic) ---
  if (alarmActive) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
 
  // General debug info
  Serial.print("G:"); Serial.print(gasValue);
  Serial.print(" | T:"); Serial.print(tempC);
  Serial.print(" | W:"); Serial.println(distance);
 
  delay(500); 
}
 
float getUltrasonicDistance() {
  pinMode(ULTRA_PIN, OUTPUT);
  digitalWrite(ULTRA_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRA_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_PIN, LOW);
 
  pinMode(ULTRA_PIN, INPUT);
  long duration = pulseIn(ULTRA_PIN, HIGH, 25000);
 
  if (duration == 0) return 10;
 
  float distance = duration * 0.034 / 2;
  return distance;
}
