#include <LiquidCrystal.h>

/***  LCD ***/
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
#define LINE_1 0
#define LINE_2 1

// Using a 16 MHz clock, we want 0.1 second interrupts();
// Timer 1 used for the Yellow Timer
// Legal values are 0 to 65536
// Prescaler 1 --> 1,600,000
// Prescaler 8 --> 200,000
// Prescaler 64 --> 25000   // Highest possible legal value!
// Prescaler 256 --> 6250
// Prescaler 1024 --> 1562.5  // Try to avoid non-integers
// Timer 2 used for the Green Timer (use 0.001 second interrupts)
// Legal values are 0 to 256
// Prescaler 8 -->
// Prescaler 32 -->
// Prescaler 64 -->

// Counter and compare values
#define TIMER_1_START 0
#define TIMER_1_COMPARE 25000

#define PIN_PUSHBUTTON_GREEN 2
#define PIN_PUSHBUTTON_YELLOW 3
#define PIN_PUSHBUTTON_BLUE 4

#define PIN_LED_GREEN 6
#define PIN_LED_YELLOW 7

/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define FLAG_PUSHBUTTON_GREEN 0x01
#define FLAG_PUSHBUTTON_YELLOW 0x02

unsigned long yellowTimerTenthsSecond = 0;
uint8_t isYellowTimerRunning = 0;

// unsigned long priorTimeMs = 0;

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

  // Timer 1 setup
  TCCR1A = 0;  // Reset Timer1 Control Reg A
  // Set to prescaler of 64
  TCCR1B &= ~_BV(CS12);
  TCCR1B |= _BV(CS11);
  TCCR1B |= _BV(CS10);
  TCNT1 = TIMER_1_START;
  OCR1A = TIMER_1_COMPARE;  // Reset Timer1 and set compare value
  TIMSK1 = _BV(OCIE1A);     // Enable Timer1 overflow interrupt
  sei();                    // Enable global interrupts
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
    yellowTimerTenthsSecond = 0;
  }

  // unsigned long currentTimeMs = millis();
  // unsigned long elapsedTimeMs = currentTimeMs - priorTimeMs;
  // priorTimeMs = currentTimeMs;
  // if (isYellowTimerRunning) {
  //   yellowTimerMs += elapsedTimeMs;
  // }
  updateLcd();
  // delay(50);
}

void updateLcd() {
  lcd.setCursor(0, LINE_1);
  lcd.print(yellowTimerTenthsSecond / 10);
  lcd.print(".");
  lcd.print(yellowTimerTenthsSecond % 10);
  //lcd.print("   ");
}

// Simple ISRs that set flags only
void green_pushbutton_isr() {
  mainEventFlags |= FLAG_PUSHBUTTON_GREEN;
}

void yellow_pushbutton_isr() {
  mainEventFlags |= FLAG_PUSHBUTTON_YELLOW;
}

ISR(TIMER1_COMPA_vect) {
  TCNT1 = TIMER_1_START;
  if (isYellowTimerRunning) {
    yellowTimerTenthsSecond++;
  }
}