#define REG_PORT_LED_BUILT_IN PORTB
#define BIT_LED_BUILT_IN 5

// Counter and compare values
#define TIMER_1_START 0
#define TIMER_1_COMPARE 31250

void setup() {
  DDRB |= _BV(BIT_LED_BUILT_IN);  // Set LED pin to be output
  TCCR1A = 0;  // Reset Timer1 Control Reg A
  // Set to prescaler of 256
  TCCR1B |= _BV(CS12);
  TCCR1B &= ~_BV(CS11);
  TCCR1B &= ~_BV(CS10);
  // TCCR1B = 0x04;  // Same code, but shorter
  TCNT1 = TIMER_1_START;
  OCR1A = TIMER_1_COMPARE;  // Reset Timer1 and set compare value
  TIMSK1 = _BV(OCIE1A);  // Enable Timer1 overflow interrupt
  sei();  // Enable global interrupts
}

void loop() {
  delay(5000);
}

ISR(TIMER1_COMPA_vect) {
  TCNT1 = TIMER_1_START;
  REG_PORT_LED_BUILT_IN ^= _BV(BIT_LED_BUILT_IN);
}
