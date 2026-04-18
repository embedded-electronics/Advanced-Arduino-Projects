// Advanced Arduino Project
// Episode 6: Digital Signal Monitor (Serial Plotter)

volatile int interval = 500;   // Delay in microseconds
int i = 0;

void setup()
{
  pinMode(7, OUTPUT);        // Output signal pin
  Serial.begin(115200);      // High speed for plotter
}

void loop()
{
  // Generate square wave
  digitalWrite(7, HIGH);
  delayMicroseconds(interval);

  digitalWrite(7, LOW);
  delayMicroseconds(interval);

  // Read analog signal (can connect potentiometer or sensor)
  int sensorValue = analogRead(A0);

  // Send data to Serial Plotter
  Serial.print(sensorValue);
  Serial.print(" ");
  Serial.println(interval);

  // Slowly vary frequency
  interval = interval + 5;

  if (interval > 1000)
    interval = 100;

  delay(10);
}
