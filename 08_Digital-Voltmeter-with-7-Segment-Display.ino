// Advanced Arduino Project
// Episode 8: Digital Voltmeter with 7-Segment Display

int input = A0;

// Display pins
int disp1 = 1;
int disp2 = 2;
int disp3 = 3;

// Segment pins
int segA = 5;
int segB = 6;
int segC = 7;
int segD = 8;
int segE = 9;
int segF = 10;
int segG = 11;
int segDP = 12;

float voltage;

void setup()
{
  pinMode(disp1, OUTPUT);
  pinMode(disp2, OUTPUT);
  pinMode(disp3, OUTPUT);

  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(segDP, OUTPUT);

  pinMode(input, INPUT);
}

void loop()
{
  int raw = analogRead(input);

  // Convert ADC to voltage (0–5V)
  voltage = raw * (5.0 / 1023.0);

  int value = voltage * 100;   // Example: 3.25 → 325

  int digit1 = value / 100;         // Hundreds
  int digit2 = (value / 10) % 10;   // Tens
  int digit3 = value % 10;          // Units

  // -------- DISPLAY DIGIT 1 --------
  digitalWrite(disp1, HIGH);
  digitalWrite(disp2, LOW);
  digitalWrite(disp3, LOW);

  digitalWrite(segDP, HIGH);   // No decimal
  display(digit1);
  delay(5);

  // -------- DISPLAY DIGIT 2 --------
  digitalWrite(disp1, LOW);
  digitalWrite(disp2, HIGH);
  digitalWrite(disp3, LOW);

  digitalWrite(segDP, LOW);    // Decimal point ON
  display(digit2);
  delay(5);

  // -------- DISPLAY DIGIT 3 --------
  digitalWrite(disp1, LOW);
  digitalWrite(disp2, LOW);
  digitalWrite(disp3, HIGH);

  digitalWrite(segDP, HIGH);   // No decimal
  display(digit3);
  delay(5);
}

// -------- SEGMENT DISPLAY FUNCTION --------
void display(int num)
{
  switch(num)
  {
    case 0: setSegments(0,0,0,0,0,0,1); break;
    case 1: setSegments(1,0,0,1,1,1,1); break;
    case 2: setSegments(0,0,1,0,0,1,0); break;
    case 3: setSegments(0,0,0,0,1,1,0); break;
    case 4: setSegments(1,0,0,1,1,0,0); break;
    case 5: setSegments(0,1,0,0,1,0,0); break;
    case 6: setSegments(0,1,0,0,0,0,0); break;
    case 7: setSegments(0,0,0,1,1,1,1); break;
    case 8: setSegments(0,0,0,0,0,0,0); break;
    case 9: setSegments(0,0,0,0,1,0,0); break;
  }
}

// -------- SEGMENT CONTROL --------
void setSegments(int A,int B,int C,int D,int E,int F,int G)
{
  digitalWrite(segA, A);
  digitalWrite(segB, B);
  digitalWrite(segC, C);
  digitalWrite(segD, D);
  digitalWrite(segE, E);
  digitalWrite(segF, F);
  digitalWrite(segG, G);
}
