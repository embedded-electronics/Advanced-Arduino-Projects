// Advanced Arduino Project
// Episode 2: Temperature Controlled Fan (Closed Loop Concept)

#define TEMPERATURE A0   // Temperature sensor (LM35)
#define MOTOR 3          // PWM pin for motor control

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR, OUTPUT);
}

// Function to decide motor speed based on temperature
int speed_decider(float temp) {

  if (temp < 20)
    return 0;            // Fan OFF

  else if (temp > 40)
    return 255;          // Full speed

  else
    return map(temp, 20, 40, 0, 255);  // Linear control
}

void loop() {

  int sensorValue = analogRead(TEMPERATURE);

  // Convert ADC value to voltage
  float voltage = sensorValue * (5.0 / 1023.0);

  // LM35: 10mV per °C
  float temperature = voltage * 100;

  int speed = speed_decider(temperature);

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | Fan Speed: ");
  Serial.println(speed);

  analogWrite(MOTOR, speed);   // PWM control

  delay(500);
}
