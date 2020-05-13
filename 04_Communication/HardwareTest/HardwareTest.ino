#define PIN_LED_RED 10
#define PIN_LED_YELLOW 9
#define PIN_LED_GREEN 6
#define PIN_LED_BLUE 5

#define PIN_PUSHBUTTON_RED 7
#define PIN_PUSHBUTTON_YELLOW 4
#define PIN_PUSHBUTTON_GREEN 3
#define PIN_PUSHBUTTON_BLUE 2

#define PIN_RGB_LED_RED 11
#define PIN_RGB_LED_GREEN 12
#define PIN_RGB_LED_BLUE 13

#define PIN_ANALOG_POT 0
#define PIN_ANALOG_PHOTOCELL 1

/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define FLAG_GREEN_INTERRUPT 0x01
#define FLAG_BLUE_INTERRUPT 0x02

int rgbCounter = 0;
unsigned long oldTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_BLUE, LOW);

  pinMode(PIN_PUSHBUTTON_RED, INPUT_PULLUP);
  pinMode(PIN_PUSHBUTTON_YELLOW, INPUT_PULLUP);
  pinMode(PIN_PUSHBUTTON_GREEN, INPUT_PULLUP);
  pinMode(PIN_PUSHBUTTON_BLUE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_GREEN), green_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_BLUE), blue_isr, FALLING);
  
  pinMode(PIN_RGB_LED_RED, OUTPUT);
  pinMode(PIN_RGB_LED_GREEN, OUTPUT);
  pinMode(PIN_RGB_LED_BLUE, OUTPUT);
  digitalWrite(PIN_RGB_LED_RED, LOW);
  digitalWrite(PIN_RGB_LED_GREEN, LOW);
  digitalWrite(PIN_RGB_LED_BLUE, LOW);
  
  // No need to setup the Analog inputs, that is the default
}

void loop() {
  int potReading = analogRead(PIN_ANALOG_POT);
  int photocellReading = analogRead(PIN_ANALOG_PHOTOCELL);
  displayAnalogInfo(potReading, photocellReading);
  
  lightPressedButtons();

  if (mainEventFlags & FLAG_GREEN_INTERRUPT) {
    delay(20);
    mainEventFlags &= ~FLAG_GREEN_INTERRUPT;
    if (!digitalRead(PIN_PUSHBUTTON_GREEN)) {
      rgbCounter = 0;
      updateRgb();
    }
  }
  if (mainEventFlags & FLAG_BLUE_INTERRUPT) {
    delay(20);
    mainEventFlags &= ~FLAG_BLUE_INTERRUPT;
    if (!digitalRead(PIN_PUSHBUTTON_BLUE)) {
      rgbCounter += 1;
      if (rgbCounter > 7) {
        rgbCounter = 0;
      }
      updateRgb();
    }
  }
  delay(100);
}

void lightPressedButtons() {
  char button_pins[] = {PIN_PUSHBUTTON_RED, PIN_PUSHBUTTON_YELLOW, PIN_PUSHBUTTON_GREEN, PIN_PUSHBUTTON_BLUE};
  char led_pins[] = {PIN_LED_RED, PIN_LED_YELLOW, PIN_LED_GREEN, PIN_LED_BLUE};
  for (int k = 0; k < sizeof(button_pins); ++k) {
    digitalWrite(led_pins[k], !digitalRead(button_pins[k]));
  }
}

void updateRgb() {
  digitalWrite(PIN_RGB_LED_RED, rgbCounter >> 2 & 0x01);
  digitalWrite(PIN_RGB_LED_GREEN, rgbCounter >> 1 & 0x01);
  digitalWrite(PIN_RGB_LED_BLUE, rgbCounter >> 0 & 0x01);
}

void displayAnalogInfo(int pot, int photocell) {
  int currentTime = millis();
  if (currentTime - oldTime > 2000) {
    oldTime = currentTime;
    Serial.print("Pot = ");
    Serial.print(pot);
    Serial.print("  Photocell = ");
    Serial.println(photocell);
  }
}

// Simple ISRs that set flags only
void blue_isr() {
  mainEventFlags |= FLAG_BLUE_INTERRUPT;
}

void green_isr() {
  mainEventFlags |= FLAG_GREEN_INTERRUPT;
}
