#include <LiquidCrystal.h>

/***  LCD ***/
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
#define LINE_1 0
#define LINE_2 1

#define PIN_PUSHBUTTON_GREEN 2
#define PIN_PUSHBUTTON_YELLOW 3
#define PIN_PUSHBUTTON_BLUE 4

#define PIN_LED_GREEN 6
#define PIN_LED_YELLOW 7

/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define FLAG_PUSHBUTTON_GREEN 0x01
#define FLAG_PUSHBUTTON_YELLOW 0x02

unsigned long yellowTimerMs = 0;
uint8_t isYellowTimerRunning = 0;

unsigned long priorTimeMs = 0;

void setup() {
  lcd.begin(16, 2);
  pinMode(PIN_PUSHBUTTON_YELLOW, INPUT_PULLUP);
  pinMode(PIN_PUSHBUTTON_GREEN, INPUT_PULLUP);
  pinMode(PIN_PUSHBUTTON_BLUE, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_GREEN), green_pushbutton_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_YELLOW), yellow_pushbutton_isr, FALLING);

  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
}

void loop() {
  if (mainEventFlags & FLAG_PUSHBUTTON_GREEN) {
    delay(20);
    mainEventFlags &= ~FLAG_PUSHBUTTON_GREEN;
    if (!digitalRead(PIN_PUSHBUTTON_GREEN)) {
      // do stuff
    }
  }
  if (mainEventFlags & FLAG_PUSHBUTTON_YELLOW) {
    delay(20);
    mainEventFlags &= ~FLAG_PUSHBUTTON_YELLOW;
    if (!digitalRead(PIN_PUSHBUTTON_YELLOW)) {
      // do stuff
      isYellowTimerRunning = !isYellowTimerRunning;
      digitalWrite(PIN_LED_YELLOW, isYellowTimerRunning);
    }
  }
  if (!digitalRead(PIN_PUSHBUTTON_BLUE)) {
    // do stuff
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_YELLOW, LOW);
    lcd.clear();
    isYellowTimerRunning = 0;
    yellowTimerMs = 0;
  }

  unsigned long currentTimeMs = millis();
  unsigned long elapsedTimeMs = currentTimeMs - priorTimeMs;
  priorTimeMs = currentTimeMs;

  if (isYellowTimerRunning) {
    yellowTimerMs += elapsedTimeMs;
  }
  updateLcd();
  delay(50);
}

void updateLcd() {
  lcd.setCursor(0, LINE_1);
  lcd.print(yellowTimerMs / 1000);
  lcd.print(".");
  lcd.print(yellowTimerMs / 100 % 10);
  //lcd.print("   ");
}

// Simple ISRs that set flags only
void green_pushbutton_isr() {
  mainEventFlags |= FLAG_PUSHBUTTON_GREEN;
}

void yellow_pushbutton_isr() {
  mainEventFlags |= FLAG_PUSHBUTTON_YELLOW;
}
