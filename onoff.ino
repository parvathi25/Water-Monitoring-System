// define LED pin
const int LED_PIN = 2;

// define push button pin
const int BUTTON_PIN = 4;

// define variable to hold LED state
bool ledState = false;

void setup() {
  // initialize LED pin as output
  pinMode(LED_PIN, OUTPUT);

  // initialize push button pin as input
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // read push button state
  int buttonState = digitalRead(BUTTON_PIN);

  // if button is pressed, toggle LED state
  if (buttonState == LOW) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    delay(100);
  }
}
//replace LED with the device 
