/***************************************
name:Stopwatch
function: you can see the number increases by one per second on the 4-digit 7-segment display.
***********************************/
//Email:support@sunfounder.com
//Website:www.sunfounder.com
/**************************************/
//#include <TimerOne.h> - this was in the original code, but don't think I need it now as we're doing everything through millis() calculations

//the pins of 4-digit 7-segment display attach to pin2-13 respectively 
int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int p = 9;
int d4 = 10;
int d3 = 11;
int d2 = 12;
int d1 = 13;
//long n = 0;// n represents the value displayed on the LED display. For example, when n=0, 0000 is displayed. The maximum value is 9999. 
int x = 100;
int del = 5;//Set del as 5; the value is the degree of fine tuning for the clock
int count = 0;//Set count=0. Here count is a count value that increases by 1 every 0.1 second, which means 1 second is counted when the value is 10

// Constants won't change.
const int buttonPin = A0;     // The number of the pushbutton pin.
const int ledPin =  A1;      // The number of the LED pin.

// Variables will change.
int buttonState = 0;         // Track the pushbutton state.
//int ledOn = 0;               // Track the LED state - don't need this either
int timing = 1;

int lastButtonState = LOW;   // Track the button state to "debounce" the signal.

unsigned long startTime = 0;           // Track of start time.
unsigned long finishTime = 0;          // Track of end time.
unsigned long elapsedTime = 0;         // Calculate elapsed time.

unsigned long lastDebounceTime = 0;  // Track the last time the output pin was toggled.
unsigned long debounceDelay = 100;    // Delay between reads to "debounce" the signal. 100 seems to work well.

void setup()
{
  //set all the pins of the LED display as output
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

  //Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  //Timer1.attachInterrupt( add ); // attach the service routine here
  
    // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  zero();

  Serial.begin(9600);

}
/***************************************/ 
void loop()
{
    // Read the state of the pushbutton value.
  buttonState = digitalRead(buttonPin);
  
  // Check to see if the button is pressed.
  // If the switch changed, due to noise or pressing:
  if (buttonState != lastButtonState) {
    // Reset the debouncing timer
    lastDebounceTime = millis();
    Serial.print("lastDB1: ");
    Serial.print(lastDebounceTime);
    Serial.print('\n');
  }
  
  // Check to see if the change in state has been longer than the delay.
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // New state has been in effect longer than debounce delay so take it as "true" value.

    // Is the button pressed?
    if (buttonState == HIGH) {
      // Yes.
      // Is the timer timing or waiting? If waiting, then kick things off with a button press, and hold things within this loop for a while.
      if (timing == 0) {
          // Timer is off so turn it on by taking a snapshot of the start time from the counter and displaying the elapsed time
          startTime = millis();

          //Turn on the timer.
          timing = 1;

          Serial.print("Timer started");
          Serial.print('\n');

          Serial.print("startTime: ");
          Serial.print(startTime);
          Serial.print('\n');

          //Now get into a loop until we hit the next button press, counting up the time and displaying it on the output.
          while (buttonState == 0){

            buttonState = digitalRead(buttonPin);
  
            // Keep checking whether the button has been pressed *inside* the while statement. 
            // If the switch changed, due to noise or pressing:
            if (buttonState != lastButtonState) {
              // Reset the debouncing timer
              lastDebounceTime = millis();
              Serial.print("lastDB2: ");
              Serial.print(lastDebounceTime);
              Serial.print('\n');
            }
            //This should kick us out of the while loop as soon as the button is pressed again.

            //The millis function never resets, so create the current elapsed time by looking at a diff between the two variables. 
            //This needs to be in another loop so that startTime isn't continually overwritten.
            elapsedTime = millis() - startTime;

            clearLEDs();//clear the 7-segment display screen
            pickDigit(0);//Light up 7-segment display d1
            pickNumber((elapsedTime/1000));// get the value of thousand
            delay(del);//delay 5ms

            clearLEDs();//clear the 7-segment display screen
            pickDigit(1);//Light up 7-segment display d2
            pickNumber((elapsedTime%1000)/100);// get the value of hundred
            delay(del);//delay 5ms

            clearLEDs();//clear the 7-segment display screen
            pickDigit(2);//Light up 7-segment display d3
            pickNumber(elapsedTime%100/10);//get the value of ten
            delay(del);//delay 5ms

            clearLEDs();//clear the 7-segment display screen
            pickDigit(3);//Light up 7-segment display d4
            pickNumber(elapsedTime%10);//Get the value of single digit
            delay(del);//delay 5ms

            Serial.print("Elapsed Time: ");
            Serial.print(elapsedTime);
            Serial.print('\n');

          }
          
      } else {
            // This is now the time when the button has been pressed again when the timer is running.
            // We now need to freeze the timer and hold that display for a few seconds, then maybe display all zeroes until we want to start again.
            clearLEDs();//clear the 7-segment display screen
            pickDigit(0);//Light up 7-segment display d1
            pickNumber((elapsedTime/1000));// get the value of thousand
            delay(del);//delay 5ms

            clearLEDs();//clear the 7-segment display screen
            pickDigit(1);//Light up 7-segment display d2
            pickNumber((elapsedTime%1000)/100);// get the value of hundred
            delay(del);//delay 5ms

            clearLEDs();//clear the 7-segment display screen
            pickDigit(2);//Light up 7-segment display d3
            pickNumber(elapsedTime%100/10);//get the value of ten
            delay(del);//delay 5ms

            clearLEDs();//clear the 7-segment display screen
            pickDigit(3);//Light up 7-segment display d4
            pickNumber(elapsedTime%10);//Get the value of single digit
            delay(del);//delay 5ms

            Serial.print("FinalTime: ");
            Serial.print(elapsedTime);
            Serial.print('\n');


            delay(5000); //Give it five seconds, then reset the timer, then back we go to the start

            //While waiting for the button to be pressed again, sit static, showing zeroes            
            zero();

            // Stop timing 
            timing = 0;
            Serial.print("Timer stopped");
            Serial.print('\n');


      }
    } else {
      // No.
      // Nothing to do.
    }
  }
  
  // Save the button state. 
  // Next time through the loop, it'll be the lastButtonState.
  lastButtonState = buttonState;
  
 }
/**************************************/ 
void pickDigit(int x) //light up a 7-segment display
{
  //The 7-segment LED display is a common-cathode one. So also use digitalWrite to  set d1 as high and the LED will go out
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);

  switch(x)
  {
    case 0: 
    digitalWrite(d1, LOW);//Light d1 up 
    break;
    case 1: 
    digitalWrite(d2, LOW); //Light d2 up 
    break;
    case 2: 
    digitalWrite(d3, LOW); //Light d3 up 
    break;
    default: 
    digitalWrite(d4, LOW); //Light d4 up 
    break;
  }
}
//The function is to control the 7-segment LED display to display numbers. Here x is the number to be displayed. It is an integer from 0 to 9 
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
void clearLEDs() //clear the 7-segment display screen
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

void zero() //the 7-segment led display 0
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void one() //the 7-segment led display 1
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void two() //the 7-segment led display 2
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}
void three() //the 7-segment led display 3
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void four() //the 7-segment led display 4
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void five() //the 7-segment led display 5
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void six() //the 7-segment led display 6
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void seven() //the 7-segment led display 7
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void eight() //the 7-segment led display 8
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void nine() //the 7-segment led display 9
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}
/*******************************************/
/* void add()
{
  // Toggle LED
  count ++;
  if(count == 10)
  {
    count = 0;
    n++;
    if(n == 10000)
    {
      n = 0;
    }
  }
}*/
