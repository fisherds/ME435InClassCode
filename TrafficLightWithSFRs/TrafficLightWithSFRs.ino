// TODO: This is not done yet!
// For the HW convert this to use SFRs


#define PIN_LED_RED 10
#define PIN_LED_YELLOW 9
#define PIN_LED_GREEN 6

#define PIN_PUSHBUTTON_BLUE 0

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);

  pinMode(PIN_PUSHBUTTON_BLUE, INPUT_PULLUP);
  redOn();
}

void loop() {
  if (digitalRead(PIN_PUSHBUTTON_BLUE) == 0) {
    greenOn();
    delay(2000);
    while (digitalRead(PIN_PUSHBUTTON_BLUE) == 0);
    yellowOn();
    delay(1500);
    redOn();
  }
  delay(100);  // Optional
}

void redOn() {
  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
}

void yellowOn() {
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_YELLOW, HIGH);
  digitalWrite(PIN_LED_GREEN, LOW);
}

void greenOn() {
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_GREEN, HIGH);
}