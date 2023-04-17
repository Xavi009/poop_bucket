/**************************************** CONSTANT DEFINITIONS *****************************************/
/*******************************************************************************************************/
const int potPin = A2;       // Analog input pin that the potentiometer is attached to
const int motorPin = 0;      // Declare motor pin; PWM; 3 for Uno; 0 for attiny85
const int ledPin = 1;        // Declare led pin; PWM; 5 for Uno; 1 for attiny85
const int button = A3;       // Declare button pin; Use analog as digital

#define UP 0                 // Define directions for LED fade
#define DOWN 1
/****************************************** GLOBAL VARIABLES *******************************************/
/*******************************************************************************************************/
int potVal, ledMode;
int ledBlink, fadeDirection;
double fadeIncrement, fadeValue;
int buttonState, previousState;
unsigned long currentMillis, newMillis;
/*************************************** FUNCTION DECLARATIONS *****************************************/
/*******************************************************************************************************/
void outputToMotor();
void changeMode();
void selectMode();
void mode1();
void mode2();
void mode3();
/*********************************************** MAIN **************************************************/
/******************************************************************************************'*************/
void setup() {
  //Serial.begin(9600);
  pinMode(button, INPUT);       // Declare button as input
  ledMode = 1;                  // Initialize LED mode to be basic on
  previousState = LOW;          // Initialize button as off
  currentMillis = millis();     // Capture the latest value of millis()
  newMillis = millis();         // Do the same as above

  ledBlink = 250;               // Period of blink mode; 250 is fastest I could get it to go
  fadeDirection = UP;           // Fade initialized as going up
  fadeValue = 0;                // Fade value initialized as off
  fadeIncrement = 0.3;          // Fade increment (modifies speed of fade)
}

void loop() {
  outputToMotor();
  buttonState = digitalRead(button);
  if (buttonState == HIGH && previousState == LOW) {   // If button is pressed, change LED mode
    changeMode();
    previousState = HIGH;                              // Change state to HIGH
  }
  if (buttonState == LOW) {                            // Prevent holding button to trigger different modes
    previousState = LOW;                               // Change state to LOW if button is LOW
  }
  selectMode();
  delay(2);
}
/************************************* FUNCTION IMPLEMENTATIONS ****************************************/
/*******************************************************************************************************/
void outputToMotor() {
  int outputVal = 0;                                  // Value to output to the PWM pin of motor
  potVal = analogRead(potPin);                        // Read the analog value of potentiometer
  outputVal = map(potVal, 0, 1023, 50, 255);          // Map the motor output to POT input
  analogWrite(motorPin, outputVal);                   // Change the PWM output value to motor
}

void changeMode() {
  // Cycles through LED modes
  currentMillis = millis();                           // Reset millis counters
  newMillis = millis();
  if (ledMode == 3) {
    ledMode = 1;
  }
  else {
    ledMode++;
  }
}

void selectMode() {
  // Calls on the appropriate mode
  if (ledMode == 1) {
    mode1();
  }
  else if (ledMode == 2) {
    mode2();
  }
  else if (ledMode == 3) {
    mode3();
  }
}


void mode1() {
  // Fully bright LED mode
  analogWrite(ledPin, 255);
}

void mode2() {
  // Blinking LED mode
  newMillis = millis();                                         // Take current millis value
  int difference = newMillis - currentMillis;
  if (difference >= (ledBlink / 2) && difference < ledBlink) {  // If in the on range, turn LED on
    analogWrite(ledPin, 255);
  }
  else if (difference >= 250) {                                 // Turn off if not
    currentMillis = millis();                                   // Reset current millis counter
    analogWrite(ledPin, 0);
  }
}

void mode3() {
  // Fading light mode
  if (fadeDirection == UP) {                                    // 
    fadeValue = fadeValue + fadeIncrement;
    if (fadeValue >= 255) {
      // At max, limit and change direction
      fadeValue = 255;
      fadeDirection = DOWN;
    }
  }
  else {
    fadeValue = fadeValue - fadeIncrement;
    if (fadeValue <= 0) {
      // At min, limit and change direction
      fadeValue = 0;
      fadeDirection = UP;
    }
    // Only need to update when it changes
  }
  analogWrite(ledPin, fadeValue);
}
