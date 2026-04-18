// Advanced Arduino Project
// Episode 4: Password-Based Door Lock System

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// ---------------- SERVO ----------------
Servo myservo;
int pos = 0;

// ---------------- LCD ----------------
// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// ---------------- KEYPAD ----------------
const byte rows = 4;
const byte cols = 3;

char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Avoid pin 1 (TX)
byte rowPins[rows] = {6, 7, 8, 9};
byte colPins[cols] = {A0, A1, A2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

// ---------------- PASSWORD ----------------
const char password[] = "1234";
int currentposition = 0;

// ---------------- OUTPUTS ----------------
#define redled 10
#define greenled 11
#define buzz 8

int invalidcount = 0;

// ---------------- SETUP ----------------
void setup()
{
  Serial.begin(9600);

  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(buzz, OUTPUT);

  myservo.attach(9);

  lcd.begin(16, 2);

  displayscreen();
}

// ---------------- LOOP ----------------
void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ENTER PASSWORD");

    lcd.setCursor(0, 1);

    // Show stars
    for (int i = 0; i < currentposition; i++)
    {
      lcd.print("*");
    }

    // Check password
    if (key == password[currentposition])
    {
      tone(buzz, 1000, 100);   // short beep
      currentposition++;

      if (currentposition == 4)
      {
        unlockdoor();
        currentposition = 0;
      }
    }
    else
    {
      invalidcount++;
      incorrect();
      currentposition = 0;
    }

    delay(200);
  }
}

// ---------------- UNLOCK DOOR ----------------
void unlockdoor()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Granted");
  lcd.setCursor(0, 1);
  lcd.print("WELCOME");

  digitalWrite(greenled, HIGH);
  tone(buzz, 1500, 200);

  // Servo open
  for (pos = 180; pos >= 0; pos -= 5)
  {
    myservo.write(pos);
    delay(10);
  }

  delay(3000);

  // Servo close
  for (pos = 0; pos <= 180; pos += 5)
  {
    myservo.write(pos);
    delay(10);
  }

  digitalWrite(greenled, LOW);

  lcd.clear();
  displayscreen();
}

// ---------------- WRONG PASSWORD ----------------
void incorrect()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wrong Password");
  lcd.setCursor(0, 1);
  lcd.print("Try Again");

  digitalWrite(redled, HIGH);
  tone(buzz, 500, 500);

  delay(2000);

  digitalWrite(redled, LOW);

  // Security lock after multiple attempts
  if (invalidcount >= 5)
  {
    lcd.clear();
    lcd.print("SYSTEM LOCKED");
    lcd.setCursor(0, 1);
    lcd.print("Wait 10 sec");

    tone(buzz, 2000);

    delay(10000);

    noTone(buzz);
    invalidcount = 0;
  }

  lcd.clear();
  displayscreen();
}

// ---------------- DISPLAY SCREEN ----------------
void displayscreen()
{
  lcd.setCursor(0, 0);
  lcd.print("*ENTER CODE*");
  lcd.setCursor(0, 1);
  lcd.print("Door Lock System");
}
