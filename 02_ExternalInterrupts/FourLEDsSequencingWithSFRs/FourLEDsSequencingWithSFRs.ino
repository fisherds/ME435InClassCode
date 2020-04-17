// THIS FILE HAS ERRORS.  It was a work in progress from the videos!

// Output PORTs and BITs
#define REG_PORT_LED_RED PORTB
#define BIT_LED_RED 2
#define REG_PORT_LED_YELLOW PORTB
#define BIT_LED_YELLOW 1
#define REG_PORT_LED_GREEN PORTD
#define BIT_LED_GREEN 6
#define REG_PORT_LED_BLUE PORTD
#define BIT_LED_BLUE 5

// Input PORTs, PIN reg, and BITs
#define REG_PORT_PUSHBUTTON_RED PORTD
#define REG_PIN_PUSHBUTTON_RED PIND
#define BIT_PUSHBUTTON_RED 3
#define REG_PORT_PUSHBUTTON_YELLOW PORTD
#define REG_PIN_PUSHBUTTON_YELLOW PIND
#define BIT_PUSHBUTTON_YELLOW 2
#define REG_PORT_PUSHBUTTON_GREEN PORTD
#define REG_PIN_PUSHBUTTON_GREEN PIND
#define BIT_PUSHBUTTON_GREEN 1
#define REG_PORT_PUSHBUTTON_BLUE PORTD
#define REG_PIN_PUSHBUTTON_BLUE PIND
#define BIT_PUSHBUTTON_BLUE 0

uint8_t greenState = HIGH;
uint8_t lastGreenState = HIGH;
uint8_t blueState = HIGH;
uint8_t lastBlueState = HIGH;

volatile uint8_t mainEventFlags = 0;
#define FLAG_RED_PUSHBUTTON 0x01
#define FLAG_YELLOW_PUSHBUTTON 0x02
#define FLAG_GREEN_PUSHBUTTON 0x04
#define FLAG_BLUE_PUSHBUTTON 0x08

volatile uint8_t portdhistory = 0xFF;  // default is high because the pull-up

void setup() {
  Serial.begin(9600);
  DDRB = _BV(BIT_LED_RED) | _BV(BIT_LED_YELLOW);  // Set RED and YELLOW LEDs as Outut
  DDRD = _BV(BIT_LED_GREEN) | _BV(BIT_LED_BLUE);  // Set GREEN and BLUE LEDs as Outut

  // All other pins are explicitly set as inputs already via the lines above.

  // Enable the four pull up resistors
  REG_PORT_PUSHBUTTON_RED |= _BV(BIT_PUSHBUTTON_RED);
  REG_PORT_PUSHBUTTON_YELLOW |= _BV(BIT_PUSHBUTTON_YELLOW);
  REG_PORT_PUSHBUTTON_GREEN |= _BV(BIT_PUSHBUTTON_GREEN);
  REG_PORT_PUSHBUTTON_BLUE |= _BV(BIT_PUSHBUTTON_BLUE);

  // Update this later!
  // attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_YELLOW), yellow_pushbutton_isr, FALLING);  // Pin 2 = RD2 = INT0
  // attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_RED), red_pushbutton_isr, FALLING);        // Pin 3 = RD3 = INT1

  // EICRA = 0x0A;  // Set INT0 and INT1 to FALLING edge interrupts
  // EICRA = _BV(ISC11) | _BV(ISC01);  // Set INT0 and INT1 to FALLING edge interrupts
  // EIMSK = 0x03;  // Turns on both INT0 and INT1
  // EIMSK = _BV(INT0) | _BV(INT1);  // Turns on both INT0 and INT1

  PCICR = _BV(PCIE2);  // Enable Pin Change Interrupts for PORT D pins
  // PCMSK2 = 0x0F;  // Enables RD0 - RD3 as interrupts
  PCMSK2 = _BV(PCINT19) | _BV(PCINT18) | _BV(PCINT17) | _BV(PCINT16);  // Enables RD0 - RD3 as interrupts

  sei();  // Turn on interrupts globally.  Not required for us, since Arduino does it.
}

void showFeedbackLeds() {
  if (bit_is_clear(REG_PIN_PUSHBUTTON_RED, BIT_PUSHBUTTON_RED)) {
    REG_PORT_LED_RED |= _BV(BIT_LED_RED);
  } else {
    REG_PORT_LED_RED &= ~_BV(BIT_LED_RED);
  }
  if (bit_is_clear(REG_PIN_PUSHBUTTON_YELLOW, BIT_PUSHBUTTON_YELLOW)) {
    REG_PORT_LED_YELLOW |= _BV(BIT_LED_YELLOW);
  } else {
    REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
  }
  if (bit_is_clear(REG_PIN_PUSHBUTTON_GREEN, BIT_PUSHBUTTON_GREEN)) {
    REG_PORT_LED_GREEN |= _BV(BIT_LED_GREEN);
  } else {
    REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
  }
  if (bit_is_clear(REG_PIN_PUSHBUTTON_BLUE, BIT_PUSHBUTTON_BLUE)) {
    REG_PORT_LED_BLUE |= _BV(BIT_LED_BLUE);
  } else {
    REG_PORT_LED_BLUE &= ~_BV(BIT_LED_BLUE);
  }
}

void oldTempTest() {
  // Test temp loop function
  REG_PORT_LED_RED |= _BV(BIT_LED_RED);
  REG_PORT_LED_YELLOW |= _BV(BIT_LED_YELLOW);
  REG_PORT_LED_GREEN |= _BV(BIT_LED_GREEN);
  REG_PORT_LED_BLUE |= _BV(BIT_LED_BLUE);
  delay(1000);

  REG_PORT_LED_RED &= ~_BV(BIT_LED_RED);
  REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
  REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
  REG_PORT_LED_BLUE &= ~_BV(BIT_LED_BLUE);
  delay(1000);
}

void loop() {
  // Keep as a reference for the Sequencing HW
  showFeedbackLeds();
  if (mainEventFlags & FLAG_RED_PUSHBUTTON) {
    delay(30);
    mainEventFlags &= ~FLAG_RED_PUSHBUTTON;
    if (bit_is_clear(REG_PIN_PUSHBUTTON_RED, BIT_PUSHBUTTON_RED)) {
      // Do the action!
      //REG_PORT_LED_RED ^= _BV(BIT_LED_RED);  // digitalWrite(PIN_LED_RED, !digitalRead(PIN_LED_RED));
      addLed(BIT_LED_RED);
    }
  }
  if (mainEventFlags & FLAG_YELLOW_PUSHBUTTON) {
    delay(30);
    mainEventFlags &= ~FLAG_YELLOW_PUSHBUTTON;
    if (bit_is_clear(REG_PIN_PUSHBUTTON_YELLOW, BIT_PUSHBUTTON_YELLOW)) {
      // Do the action!
      //REG_PORT_LED_YELLOW ^= _BV(BIT_LED_YELLOW);  // digitalWrite(PIN_LED_YELLOW, !digitalRead(PIN_LED_YELLOW));
      addLed(BIT_LED_YELLOW);
    }
  }
  if (mainEventFlags & FLAG_GREEN_PUSHBUTTON) {
    delay(30);
    mainEventFlags &= ~FLAG_GREEN_PUSHBUTTON;
    if (bit_is_clear(REG_PIN_PUSHBUTTON_GREEN, BIT_PUSHBUTTON_GREEN)) {
      // Do the action!
      //REG_PORT_LED_GREEN ^= _BV(BIT_LED_GREEN);  // digitalWrite(PIN_LED_GREEN, !digitalRead(PIN_LED_GREEN));
      addLed(BIT_LED_GREEN);
    }
  }
  if (mainEventFlags & FLAG_BLUE_PUSHBUTTON) {
    delay(30);
    mainEventFlags &= ~FLAG_BLUE_PUSHBUTTON;
    if (bit_is_clear(REG_PIN_PUSHBUTTON_BLUE, BIT_PUSHBUTTON_BLUE)) {
      // Do the action!
      //REG_PORT_LED_BLUE ^= _BV(BIT_LED_BLUE);  // digitalWrite(PIN_LED_BLUE, !digitalRead(PIN_LED_BLUE));
      runSequence();
    }
  }
}

void resetArray() {
  currentIndex = 0;
  savedLeds[0] = PIN_LED_BLUE;
  savedLeds[1] = PIN_LED_BLUE;
  savedLeds[2] = PIN_LED_BLUE;
  savedLeds[3] = PIN_LED_BLUE;
  savedLeds[4] = PIN_LED_BLUE;
  savedLeds[5] = PIN_LED_BLUE;
  savedLeds[6] = PIN_LED_BLUE;
  savedLeds[7] = PIN_LED_BLUE;
  savedLeds[8] = PIN_LED_BLUE;
  savedLeds[9] = PIN_LED_BLUE;
}

void addLed(uint8_t newLedPin) {
  if (currentIndex < sizeof(savedLeds)) {
    savedLeds[currentIndex] = newLedPin;
    currentIndex++;
  }
}

void runSequence() {
  // digitalWrite(PIN_LED_BLUE, LOW);
  REG_PORT_LED_BLUE &= ~_BV(BIT_LED_BLUE);
  for (int k = 0; k < sizeof(savedLeds); k++) {
    uint8_t activeLedBit = savedLeds[k];
    // digitalWrite(activeLedPin, HIGH);
    if (activeLedBit == BIT_LED_RED || activeLedBit == BIT_LED_RED) {
      REG_PORT_LED_RED |= _BV(activeLedBit);
    } else {
      REG_PORT_LED_GREEN |= _BV(activeLedBit);
    }
    delay(1000);
    // digitalWrite(activeLedPin, LOW);
    if (activeLedBit == BIT_LED_RED || activeLedBit == BIT_LED_RED) {
      REG_PORT_LED_RED &= ~_BV(activeLedBit);
    } else {
      REG_PORT_LED_GREEN &= ~_BV(activeLedBit);
    }
    delay(100);
  }
  resetArray();
}

ISR(PCINT2_vect) {
  // ISR called means that RD0, RD1, RD2, or RD3 changed.
  uint8_t changedbits;
  changedbits = PIND ^ portdhistory;  // All pins are on PORT D
  portdhistory = PIND;
  if (changedbits & _BV(BIT_PUSHBUTTON_RED)) {
    if (bit_is_clear(REG_PIN_PUSHBUTTON_RED, BIT_PUSHBUTTON_RED)) {
      mainEventFlags |= FLAG_RED_PUSHBUTTON;
    }
  }
  if (changedbits & _BV(BIT_PUSHBUTTON_YELLOW)) {
    if (bit_is_clear(REG_PIN_PUSHBUTTON_YELLOW, BIT_PUSHBUTTON_YELLOW)) {
      mainEventFlags |= FLAG_YELLOW_PUSHBUTTON;
    }
  }
  if (changedbits & _BV(BIT_PUSHBUTTON_GREEN)) {
    if (bit_is_clear(REG_PIN_PUSHBUTTON_GREEN, BIT_PUSHBUTTON_GREEN)) {
      mainEventFlags |= FLAG_GREEN_PUSHBUTTON;
    }
  }
  if (changedbits & _BV(BIT_PUSHBUTTON_BLUE)) {
    if (bit_is_clear(REG_PIN_PUSHBUTTON_BLUE, BIT_PUSHBUTTON_BLUE)) {
      mainEventFlags |= FLAG_BLUE_PUSHBUTTON;
    }
  }
}