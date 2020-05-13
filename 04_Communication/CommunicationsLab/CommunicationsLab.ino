#define PIN_ANALOG_POT 0
int potReading;

String inputString = "";      // a String to hold incoming data
volatile bool stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(9600);
  inputString.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (stringComplete) {
    // Do stuff with the message.
    if (inputString.equalsIgnoreCase("ON")) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else if (inputString.equalsIgnoreCase("OFF")) {
      digitalWrite(LED_BUILTIN, LOW);
    } else if (inputString.equalsIgnoreCase("ADC")) {
      potReading = analogRead(PIN_ANALOG_POT);
      Serial.print("POT ");
      Serial.println(potReading);
    }

    // Prepare for the next message
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}
