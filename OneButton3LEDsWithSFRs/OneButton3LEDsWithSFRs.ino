#define PIN_LED_RED 10
#define PIN_LED_YELLOW		9
#define PIN_LED_GREEN		6
#define PIN_LED_BLUE		5

#define PRESSED 0
#define UNPRESSED 1

#define PIN_PUSHBUTTON_BLUE	0

void setup() {
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  
  pinMode(PIN_PUSHBUTTON_BLUE, INPUT_PULLUP);  
}

void loop() {
  if (digitalRead(PIN_PUSHBUTTON_BLUE) == PRESSED) {
    // Other ways that work.
    //PORTB = PORTB | B00000100;
    // PORTB = PORTB | 0x04;
    // PORTB |= 0x04;
    // PORTB |= (1 << PORTB2);
    PORTB |= _BV(PORTB2);   //replaces digitalWrite(PIN_LED_RED, HIGH);
    PORTB |= _BV(PORTB1); // digitalWrite(PIN_LED_YELLOW, HIGH);
    PORTD |= _BV(PORTD6); // digitalWrite(PIN_LED_GREEN, HIGH);
  } else {
    // PORTB = PORTB & B11111011;
    PORTB &= ~_BV(PORTB2);  //digitalWrite(PIN_LED_RED, LOW);
    PORTB &= ~_BV(PORTB1);  // digitalWrite(PIN_LED_YELLOW, LOW);
    PORTD &= ~_BV(PORTD6);  // digitalWrite(PIN_LED_GREEN, LOW);
  }
}