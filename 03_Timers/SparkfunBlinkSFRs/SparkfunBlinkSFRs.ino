// Pins
const int led_pin = PB5;

// Counter and compare values
const uint16_t t1_load = 0;
const uint16_t t1_comp = 31250;

void setup() {

  // Set LED pin to be output
  DDRB |= (1 << led_pin);

  // Reset Timer1 Control Reg A
  TCCR1A = 0;

  // Set to prescaler of 256
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);

  // Reset Timer1 and set compare value
  TCNT1 = t1_load;
  OCR1A = t1_comp;

  // Enable Timer1 overflow interrupt
  TIMSK1 = (1 << OCIE1A);

  // Enable global interrupts
  sei();
}

void loop() {
  delay(500);
}

ISR(TIMER1_COMPA_vect) {
  TCNT1 = t1_load;
  PORTB ^= (1 << led_pin);
}