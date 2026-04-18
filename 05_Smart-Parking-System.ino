// Advanced Arduino Project
// Episode 5: Smart Parking System

#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>

Adafruit_LiquidCrystal lcd(0x27);
Servo myservo;

// Ultrasonic Pins
const int entryTrigPin = 5;
const int entryEchoPin = 6;
const int exitTrigPin  = 7;
const int exitEchoPin  = 8;

const int DISTANCE_THRESHOLD = 20; // cm (more realistic)

// Parking Slots
int totalSlots = 5;
int availableSlots = 5;

// Flags
bool entryDetected = false;
bool exitDetected  = false;

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setBacklight(1);

  pinMode(entryTrigPin, OUTPUT);
  pinMode(entryEchoPin, INPUT);
  pinMode(exitTrigPin, OUTPUT);
  pinMode(exitEchoPin, INPUT);

  myservo.attach(4);
  myservo.write(90);  // Gate closed

  lcd.setCursor(0, 0);
  lcd.print("SMART PARKING");
  delay(2000);
  lcd.clear();
}

// -------- ULTRASONIC FUNCTION --------
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  long distance = duration * 0.034 / 2;
  return distance;
}

// -------- LOOP --------
void loop() {

  long entryDistance = measureDistance(entryTrigPin, entryEchoPin);
  long exitDistance  = measureDistance(exitTrigPin, exitEchoPin);

  // -------- ENTRY LOGIC --------
  if (entryDistance > 0 && entryDistance < DISTANCE_THRESHOLD && !entryDetected) {
    entryDetected = true;

    if (availableSlots > 0) {
      openGate();
      availableSlots--;
    } else {
      lcd.clear();
      lcd.print("Parking FULL");
      delay(2000);
    }
  }

  // -------- EXIT LOGIC --------
  if (exitDistance > 0 && exitDistance < DISTANCE_THRESHOLD && !exitDetected) {
    exitDetected = true;

    if (availableSlots < totalSlots) {
      openGate();
      availableSlots++;
    }
  }

  // -------- RESET FLAGS --------
  if (entryDistance > DISTANCE_THRESHOLD) entryDetected = false;
  if (exitDistance > DISTANCE_THRESHOLD) exitDetected = false;

  // -------- DISPLAY --------
  lcd.setCursor(0, 0);
  lcd.print("WELCOME");

  lcd.setCursor(0, 1);
  lcd.print("Slots: ");
  lcd.print(availableSlots);
  lcd.print("   ");

  delay(200);
}

// -------- SERVO FUNCTION --------
void openGate() {
  myservo.write(0);    // Open gate
  delay(2000);

  myservo.write(90);   // Close gate
}
