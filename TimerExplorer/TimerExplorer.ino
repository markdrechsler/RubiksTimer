/**
 * TimerExplorer.
 *
 * Explore how timing works on an Aduino.
 */

/**
 * Include the TimerOne library.
 *
 * This will be used to keep track of time.
 */
#include <TimerOne.h>

/**
 * Define constants.
 *
 * Constants declard here have global scope, accessible to all functions.
 *
 * Constants are used to prevent acidental changes to pin assignments.
 */
// Constants for the pins used by the 4-digit 7-segment display.
const int a = 2;
const int b = 3;
const int c = 4;
const int d = 5;
const int e = 6;
const int f = 7;
const int g = 8;
const int p = 9;
const int d4 = 10;
const int d3 = 11;
const int d2 = 12;
const int d1 = 13;

// Constants to define the four digits in the display.
const int firstDigit = 0;
const int secondDigit = 1;
const int thirdDigit = 2;
const int fourthDigit = 3;

// Constant to define serial baud rate.
const int serialBaud = 9600;

// Constant defining 100000 microseconds (0.1 seconds).
const long timerLength = 100000;

// Constants to define start of sequence.
// Display zero with no running timer.
const int sequenceStart = 0;

// Display time with running time.
const int sequenceMiddle = 1;

// Display time, with no running time.
const int sequenceEnd = 2;

// The number of milliseconds between reads to 'debounce' button presses.
const unsigned long debounceDelay = 500;

// The pin connected to the button.
const uint8_t buttonPin = A0;

/**
 * Define variables.
 *
 * Variables declard here have global scope, accessible to all functions.
 */
// The number of seconds that has elapsed.
long elapsedSeconds = 0;

// The number of milliseconds delay between updating each digit of the display.
int displayRefreshDelay = 5;

// The number of fractional seconds. Increases by 1 every 0.1 second,
int fractionalSeconds = 0;

// Keep track of the sequence of play events.
int sequencePosition = 0; 

// Variables to debounce button presses. 
int btnCurrentReading;
int btnPreviousReading = HIGH;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long btnTime = 0;         // the last time the output pin was toggled
long btnDebounceDelay = 200;   // the debounce time, increase if the output flickers

/**
 * Setup the program.
 *
 * Runs once, and only once, at the very beginning of execution.
 */
void setup()
{
  // Setup all of the pins used by the 4-digit 7-segment display for output.
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(p, OUTPUT);

  // Setup the serial connection to be 9600 baud (pits per second).
  Serial.begin(serialBaud);

  // Setup the  pushbutton pin as an input:
  pinMode(buttonPin, INPUT); 

  // Initialise a timer.
  Timer1.initialize(timerLength);
}

/**
 * Main body of the program.
 *
 * Display the number of elapsed seconds.
 */
void loop()
{
  // Get the current reading from the button.
  btnCurrentReading = digitalRead(buttonPin);

  // If the button just went from HIGH to LOW and we've waited long enough
  // to ignore any noise on the circuit, declare the button pressed.
  if (btnCurrentReading == HIGH && btnPreviousReading == LOW && millis() - btnTime > btnDebounceDelay) {
    // The button has been pressed.
    Serial.println("Button pressed");

    // Work out where we are in the sequence.
    switch(sequencePosition)
    {
      case sequenceStart:
      // Start of the sequence.
      // Start the timer running.
      // Everytime the timer fires, run the specified function.
      Timer1.attachInterrupt(addFractionalSecond);
      
      break;
      
      case sequenceMiddle:
      // Middle of the sequence. Stop the timer.
      // Detatch our function from the timer.
      Timer1.detachInterrupt();
      break;

      case sequenceEnd:
      // End of the sequence.
      // Reset the seconds count.
      fractionalSeconds = 0;
      elapsedSeconds = 0;
      break;
    }

    // Increment the sequence position.
    sequencePosition++;

    // Wrap around the start if we've finished the sequence.
    if (sequencePosition > sequenceEnd) {
      sequencePosition = 0;
    }
  }

  btnPreviousReading = btnCurrentReading;

  /**
   * Repeat the same basic four steps.
   *
   * - Clear the display.
   * - Choose a digit to update.
   * - Choose the number to display.
   * - Delay execution for specific millisecons.
   */
  clearLEDs();
  pickDigit(firstDigit);
  pickNumber((elapsedSeconds%1000)/100);// Hundreds of seconds.
  delay(displayRefreshDelay);

  clearLEDs();
  pickDigit(secondDigit);
  pickNumber((elapsedSeconds%100/10));// Tens of seconds.
  delay(displayRefreshDelay);

  clearLEDs();
  pickDigit(thirdDigit);
  pickNumber((elapsedSeconds%10));// Individual of seconds.
  decimalPoint();
  delay(displayRefreshDelay);

  clearLEDs();
  pickDigit(fourthDigit);
  pickNumber(fractionalSeconds);// Fractions of a second.
  delay(displayRefreshDelay);

}

/**
 * Count the number of fractional seconds that have elapsed.
 *
 * Triggered by the timer interrupt configured in setup
 */
 void addFractionalSecond()
 {
   // Increment the fractional seconds count.
   fractionalSeconds ++;

   // Have 10 fractional seconds have passed?
   if(fractionalSeconds == 10)
   {
     // Yes. Entire second has elapsed.
     // Reset the fractional seconds count.
     fractionalSeconds = 0;

     // Increment the number of elapsed seconds to display.
     elapsedSeconds++;

     // Output some debugging code to help.
     Serial.println("A second has elapsed");

     // Is the number of elapsed seconds to high to display?
     if(elapsedSeconds == 10000)
     {
       // Yes.
       // Reset the elapsed number of seconds.
       elapsedSeconds = 0;
     }
   }
}

/**
 * Utility function to turn on the decimal point
 */
void decimalPoint()
{
  digitalWrite(p, HIGH);
}

/**
 * Utility functions to control the 4-digit 7-segment display.
 *
 * Based on code found here:
 * http://bit.ly/2hK5DSc
 *
 * There should be no need to edit code below here.
 */

/**
 * Determine which digit in the 4-digit 7-segment display will be updated.
 *
 * The 7-segment LED display is a common-cathode one.
 * Use digitalWrite to set a pin HIGH to turn the digit off.
 */
 void pickDigit(int digit)
 {
   // Turn off all digits.
   digitalWrite(d1, HIGH);
   digitalWrite(d2, HIGH);
   digitalWrite(d3, HIGH);
   digitalWrite(d4, HIGH);

   // Determine which digit to turn on.
   switch(digit)
   {
     case 0:
     digitalWrite(d1, LOW); // Turn on first digit.
     break;
     case 1:
     digitalWrite(d2, LOW); // Turn on second digit.
     break;
     case 2:
     digitalWrite(d3, LOW); // Turn on third digit.
     break;
     default:
     digitalWrite(d4, LOW); // Turn on forth digit.
     break;
   }
 }

/**
 * Turn off all of the LEDs in one of the 7-sgment displays.
 */
void clearLEDs()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(p, LOW);
}

/**
 * Determine which number to display.
 */
void pickNumber(int x)
{
  switch(x)
  {
    default:
    zero();
    break;
    case 1:
    one();
    break;
    case 2:
    two();
    break;
    case 3:
    three();
    break;
    case 4:
    four();
    break;
    case 5:
    five();
    break;
    case 6:
    six();
    break;
    case 7:
    seven();
    break;
    case 8:
    eight();
    break;
    case 9:
    nine();
    break;
  }
}

/**
 * Make one of the 7-segment displays show 0.
 */
void zero()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

/**
 * Make one of the 7-segment displays show 1.
 */
void one()
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

/**
 * Make one of the 7-segment displays show 2.
 */
void two()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

/**
 * Make one of the 7-segment displays show 3.
 */
void three()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

/**
 * Make one of the 7-segment displays show 4.
 */
void four()
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

/**
 * Make one of the 7-segment displays show 5.
 */
void five()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

/**
 * Make one of the 7-segment displays show 6.
 */
void six()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

/**
 * Make one of the 7-segment displays show 7.
 */
void seven()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

/**
 * Make one of the 7-segment displays show 8.
 */
void eight()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

/**
 * Make one of the 7-segment displays show 9.
 */
void nine()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}
