#define PIN_LED_RED 10
#define PIN_LED_YELLOW 9
#define PIN_LED_GREEN 6
#define PIN_LED_BLUE 5

#define PIN_PUSHBUTTON_RED 3
#define PIN_PUSHBUTTON_YELLOW 2
#define PIN_PUSHBUTTON_GREEN 1
#define PIN_PUSHBUTTON_BLUE 0

/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define FLAG_YELLOW_PUSHBUTTON 0x01
#define FLAG_RED_PUSHBUTTON 0x02

uint8_t greenState = 1;
uint8_t blueState = 1;
uint8_t lastGreenState = 1;
uint8_t lastBlueState = 1;

uint8_t currentIndex = 0;
uint8_t savedLeds[10] = {PIN_LED_BLUE,
                        PIN_LED_BLUE,
                        PIN_LED_BLUE,
                        PIN_LED_BLUE,
                        PIN_LED_BLUE,
                        PIN_LED_BLUE,
                        PIN_LED_BLUE,
                        PIN_LED_BLUE,
                        PIN_LED_BLUE,
                        PIN_LED_BLUE};

uint8_t ledPins[4] = {PIN_LED_RED,
                        PIN_LED_YELLOW,
                        PIN_LED_GREEN,
                        PIN_LED_BLUE};

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);

  pinMode(PIN_PUSHBUTTON_RED, INPUT_PULLUP);
  pinMode(PIN_PUSHBUTTON_YELLOW, INPUT_PULLUP);
  pinMode(PIN_PUSHBUTTON_GREEN, INPUT_PULLUP);
  pinMode(PIN_PUSHBUTTON_BLUE, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_YELLOW), yellow_pushbutton_isr, FALLING);  // RD2 i.e. 2 (isr 0)
  attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_RED), red_pushbutton_isr, FALLING);  // RD3 i.e. 3 (isr 1)
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

void loop()
{
  digitalWrite(PIN_LED_RED, !digitalRead(PIN_PUSHBUTTON_RED));
  digitalWrite(PIN_LED_YELLOW, !digitalRead(PIN_PUSHBUTTON_YELLOW));
  digitalWrite(PIN_LED_GREEN, !digitalRead(PIN_PUSHBUTTON_GREEN));
  digitalWrite(PIN_LED_BLUE, !digitalRead(PIN_PUSHBUTTON_BLUE));
  
  if (mainEventFlags & FLAG_RED_PUSHBUTTON) {
    delay(20);
    mainEventFlags &= ~FLAG_RED_PUSHBUTTON;
    if (!digitalRead(PIN_PUSHBUTTON_RED)) {
      addLed(PIN_LED_RED);
    }
  }
  if (mainEventFlags & FLAG_YELLOW_PUSHBUTTON) {
    delay(20);
    mainEventFlags &= ~FLAG_YELLOW_PUSHBUTTON;
    if (!digitalRead(PIN_PUSHBUTTON_YELLOW)) {
      addLed(PIN_LED_YELLOW);
    }
  }

  greenState = digitalRead(PIN_PUSHBUTTON_GREEN);
  if (greenState != lastGreenState) {
    if (!greenState) {
      addLed(PIN_LED_GREEN);
    }
    delay(50);  // Delay a little bit to avoid bouncing
  }
  lastGreenState = greenState;

  blueState = digitalRead(PIN_PUSHBUTTON_BLUE);
  if (blueState != lastBlueState) {
    if (!blueState) {
      runSequence();
    }
    delay(50);  // Delay a little bit to avoid bouncing
  }
  lastBlueState = blueState;
}

void addLed(uint8_t newLedPin) {
  if (currentIndex < sizeof(savedLeds)) {
    savedLeds[currentIndex] = newLedPin;
    currentIndex++;
  }
}

void runSequence() {
  digitalWrite(PIN_LED_BLUE, LOW);
  for (int k = 0; k < sizeof(savedLeds); k++) {
    uint8_t activeLedPin = savedLeds[k];
    digitalWrite(activeLedPin, HIGH);
    delay(1000);
    digitalWrite(activeLedPin, LOW);
    delay(100);
  }
  resetArray();
}

// Simple ISRs that set flags only
void yellow_pushbutton_isr() {
  mainEventFlags |= FLAG_YELLOW_PUSHBUTTON;
}

void red_pushbutton_isr() {
  mainEventFlags |= FLAG_RED_PUSHBUTTON;
}
