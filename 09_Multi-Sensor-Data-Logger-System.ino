// Advanced Arduino Project
// Episode 9: Multi-Sensor Data Logger System

// -------- PIN DEFINITIONS --------
int tempSensor  = A0;
int lightSensor = A1;
int forceSensor = A2;
int soilSensor  = A3;
int gasSensor   = A4;

int triggerPin = 2;
int echoPin    = 7;

long duration;
float distance_cm;

// -------- SETUP --------
void setup()
{
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);  // Faster for logging
}

// -------- LOOP --------
void loop()
{
  // 🌡️ Temperature (LM35/TMP36)
  int tempReading = analogRead(tempSensor);
  float voltage = tempReading * (5.0 / 1023.0);
  float tempC = (voltage - 0.5) * 100;

  // 🌞 Light Sensor
  int lightReading = analogRead(lightSensor);

  // 💪 Force Sensor
  int forceValue = analogRead(forceSensor);

  // 🌱 Soil Moisture (Convert to %)
  int soilRaw = analogRead(soilSensor);
  int soilPercent = map(soilRaw, 1023, 0, 0, 100);

  // 🌫️ Gas Sensor
  int gasValue = analogRead(gasSensor);

  // 📏 Ultrasonic Distance
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  distance_cm = duration * 0.034 / 2;

  // -------- SERIAL LOG FORMAT --------
  Serial.print("Temp:"); Serial.print(tempC); Serial.print("C | ");
  Serial.print("Light:"); Serial.print(lightReading); Serial.print(" | ");
  Serial.print("Force:"); Serial.print(forceValue); Serial.print(" | ");
  Serial.print("Soil:"); Serial.print(soilPercent); Serial.print("% | ");
  Serial.print("Gas:"); Serial.print(gasValue); Serial.print(" | ");
  Serial.print("Dist:"); Serial.print(distance_cm); Serial.println("cm");

  delay(1000);
}
