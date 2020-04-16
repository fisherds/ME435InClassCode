#define PIN_LED_RED 10
#define PIN_LED_YELLOW 9
#define PIN_LED_GREEN 6
#define PIN_LED_BLUE 5

// Can be used for INT1
#define PIN_PUSHBUTTON_RED 3   
// Can be used for INT0
#define PIN_PUSHBUTTON_YELLOW 2
#define PIN_PUSHBUTTON_GREEN 1
#define PIN_PUSHBUTTON_BLUE 0

uint8_t greenState = HIGH;
uint8_t lastGreenState = HIGH;
uint8_t blueState = HIGH;
uint8_t lastBlueState = HIGH;

volatile uint8_t mainEventFlags = 0;
#define FLAG_YELLOW_PUSHBUTTON 0x01
#define FLAG_RED_PUSHBUTTON 0x02


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
  
  attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_YELLOW), yellow_pushbutton_isr, FALLING);  // Pin 2 = RD2 = INT0
  attachInterrupt(digitalPinToInterrupt(PIN_PUSHBUTTON_RED), red_pushbutton_isr, FALLING);  // Pin 3 = RD3 = INT1
}

void loop()
{
  // Keep as a reference for the Sequencing HW
//  digitalWrite(PIN_LED_RED, !digitalRead(PIN_PUSHBUTTON_RED));
//  digitalWrite(PIN_LED_YELLOW, !digitalRead(PIN_PUSHBUTTON_YELLOW));
//  digitalWrite(PIN_LED_GREEN, !digitalRead(PIN_PUSHBUTTON_GREEN));
//  digitalWrite(PIN_LED_BLUE, !digitalRead(PIN_PUSHBUTTON_BLUE));
  
  if (mainEventFlags & FLAG_RED_PUSHBUTTON) {
    delay(30);
    mainEventFlags &= ~FLAG_RED_PUSHBUTTON;
    if (!digitalRead(PIN_PUSHBUTTON_RED)) {
      // Do the action!
      digitalWrite(PIN_LED_RED, !digitalRead(PIN_LED_RED));
    }
  }
  
  if (mainEventFlags & FLAG_YELLOW_PUSHBUTTON) {  
    delay(30);
    mainEventFlags &= ~FLAG_YELLOW_PUSHBUTTON;
    if (!digitalRead(PIN_PUSHBUTTON_YELLOW)) {    
      // Do the action!
      digitalWrite(PIN_LED_YELLOW, !digitalRead(PIN_LED_YELLOW));
    }
  }
  
  greenState = digitalRead(PIN_PUSHBUTTON_GREEN);
  if (greenState != lastGreenState) {
    if (!greenState) {
      // Do the action!
      digitalWrite(PIN_LED_GREEN, !digitalRead(PIN_LED_GREEN));
    }
    delay(50);
  }
  lastGreenState = greenState;
  
  blueState = digitalRead(PIN_PUSHBUTTON_BLUE);
  if (blueState != lastBlueState) {
    if (!blueState) {
      // Do the action!
      digitalWrite(PIN_LED_BLUE, !digitalRead(PIN_LED_BLUE));
    }
    delay(50);
  }
  lastBlueState = blueState;
}

void yellow_pushbutton_isr() {
  mainEventFlags |= FLAG_YELLOW_PUSHBUTTON;
}

void red_pushbutton_isr() {
  mainEventFlags |= FLAG_RED_PUSHBUTTON;
}

