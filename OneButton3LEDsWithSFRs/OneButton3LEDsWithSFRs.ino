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
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_LED_YELLOW, HIGH);
    digitalWrite(PIN_LED_GREEN, HIGH);
  } else {
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_YELLOW, LOW);
    digitalWrite(PIN_LED_GREEN, LOW);
  }
}