// Advanced Arduino Project
// Episode 3: Water Tank Monitoring with LCD

#include <LiquidCrystal.h>
#include <EEPROM.h>

// LCD Pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Buttons (using internal pull-up)
#define menuBtn  A4
#define plusBtn  8
#define minusBtn 9
#define saveBtn  6

#define motorPin A5

// Ultrasonic Pins (recommended)
#define TRIG 7
#define ECHO 7   // (still same, but better to separate in real design)

// Pages
#define homePage 0
#define menuPage 1

uint8_t page = homePage;
uint8_t getLevelFlag = 0;

// Load tank size from EEPROM
uint8_t tank_size = EEPROM.read(0);

int cm = 0;
float feet = 0;
uint8_t waterLevel = 0;

// ---------------- ULTRASONIC FUNCTION ----------------
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  pinMode(echoPin, INPUT);

  return pulseIn(echoPin, HIGH);
}

// ---------------- SETUP ----------------
void setup()
{
  pinMode(menuBtn, INPUT_PULLUP);
  pinMode(plusBtn, INPUT_PULLUP);
  pinMode(minusBtn, INPUT_PULLUP);
  pinMode(saveBtn, INPUT_PULLUP);

  pinMode(motorPin, OUTPUT);

  Serial.begin(9600);

  lcd.begin(16, 2);

  // Default tank size check
  if (tank_size == 0 || tank_size > 10)
    tank_size = 5;

  lcd.setCursor(0, 0);
  lcd.print("Water System");
  delay(1000);

  openHomePage();
}

// ---------------- LOOP ----------------
void loop()
{
  // -------- WATER LEVEL CALCULATION --------
  if (page == homePage && getLevelFlag == 1)
  {
    cm = 0.01723 * readUltrasonicDistance(TRIG, ECHO);

    feet = (cm / 2.54) / 12;

    if (feet > tank_size) feet = tank_size;

    waterLevel = ((tank_size - feet) / tank_size) * 100;

    if (waterLevel < 0) waterLevel = 0;
    if (waterLevel > 100) waterLevel = 100;

    // Display on LCD
    lcd.setCursor(6, 1);
    lcd.print("   ");
    lcd.setCursor(6, 1);
    lcd.print(waterLevel);
    lcd.print("%");

    // Motor Control Logic
    if (waterLevel == 100)
      digitalWrite(motorPin, LOW);   // OFF
    else if (waterLevel < 20)
      digitalWrite(motorPin, HIGH);  // ON
  }

  // -------- MENU BUTTON --------
  if (!digitalRead(menuBtn))
  {
    if (page != menuPage)
      openMenuPage();
    else
      openHomePage();

    delay(200);
  }

  // -------- INCREASE SIZE --------
  if (!digitalRead(plusBtn) && page == menuPage)
  {
    tank_size++;
    displayTankSize();
    delay(200);
  }

  // -------- DECREASE SIZE --------
  if (!digitalRead(minusBtn) && page == menuPage)
  {
    tank_size--;
    displayTankSize();
    delay(200);
  }

  // -------- SAVE --------
  if (!digitalRead(saveBtn) && page == menuPage)
  {
    EEPROM.write(0, tank_size);
    openHomePage();
    delay(200);
  }
}

// ---------------- HOME PAGE ----------------
void openHomePage()
{
  page = homePage;

  if (!tank_size)
  {
    getLevelFlag = 0;
    lcd.clear();
    lcd.print("Set Tank Size");
  }
  else
  {
    getLevelFlag = 1;
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Tank Level");
  }
}

// ---------------- MENU PAGE ----------------
void openMenuPage()
{
  getLevelFlag = 0;
  page = menuPage;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Tank Size");

  displayTankSize();
}

// ---------------- DISPLAY SIZE ----------------
void displayTankSize()
{
  if (tank_size > 10) tank_size = 10;
  if (tank_size < 1) tank_size = 1;

  lcd.setCursor(4, 1);
  lcd.print("   ");
  lcd.setCursor(4, 1);

  lcd.print(tank_size);
  lcd.print(" ft");
} 
