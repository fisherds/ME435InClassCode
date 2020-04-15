// #define PIN_LED_RED 10
// #define PIN_LED_YELLOW		9
// #define PIN_LED_GREEN		6

// #define PRESSED 0
// #define UNPRESSED 1

// #define PIN_PUSHBUTTON_BLUE	0

void setup()
{
  DDRB |= _BV(DDB2); //pinMode(PIN_LED_RED, OUTPUT);   // Output = 1, Input = 0
  DDRB |= _BV(DDB1); // pinMode(PIN_LED_YELLOW, OUTPUT);
  DDRD |= _BV(DDD6); // pinMode(PIN_LED_GREEN, OUTPUT);

  // Another option:
  // DDRB = 0x06;  // Set pins 1 and 2 as outputs, others as inputs
  // DDRD = 0x40;  // Set pin 6 as an output, others as inputs

  // Takes 2 lines to do this library function // pinMode(PIN_PUSHBUTTON_BLUE, INPUT_PULLUP);
  DDRD &= ~_BV(DDD0);   // Make it an input explicitly even though this is the default.
  PORTD |= _BV(PORTD0); // Make it a pull UP resistor enabled
}

void loop()
{
  // if (digitalRead(PIN_PUSHBUTTON_BLUE) == PRESSED) {
  // if (!digitalRead(PIN_PUSHBUTTON_BLUE))
  //if (!(PIND & 0x01))
  // if (!(PIND & _BV(PIND0)))
  if (bit_is_clear(PIND, PIND0))
  {
    // Other ways that work.
    //PORTB = PORTB | B00000100;
    // PORTB = PORTB | 0x04;
    // PORTB |= 0x04;
    // PORTB |= (1 << PORTB2);
    PORTB |= _BV(PORTB2); //replaces digitalWrite(PIN_LED_RED, HIGH);
    PORTB |= _BV(PORTB1); // digitalWrite(PIN_LED_YELLOW, HIGH);
    PORTD |= _BV(PORTD6); // digitalWrite(PIN_LED_GREEN, HIGH);
  }
  else
  {
    // PORTB = PORTB & B11111011;
    PORTB &= ~_BV(PORTB2); //digitalWrite(PIN_LED_RED, LOW);
    PORTB &= ~_BV(PORTB1); // digitalWrite(PIN_LED_YELLOW, LOW);
    PORTD &= ~_BV(PORTD6); // digitalWrite(PIN_LED_GREEN, LOW);
  }
}