// Code v1. Unoptimized, messy, prototypical rough draft for my proof of concept. Padlock works, 
// LED user feedback works as well, and updating LCD messages. Servo not connected yet, but should be simple addition.

#include <Adafruit_NeoPixel.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const int button1 = 2; //first button is on pin 8
const int button2 = 3; //second is on pin 9
const int button3 = 4; //third is pin 10
const int button4 = 6; //fourth is pin 11
const int button5 = 5; //third is pin 10
const int button6 = 7; //fourth is pin 11
const int Red = 11; //red LED is on pin 4
const int greenLed = 8; //green LED is pin 12
void checkEntered1(int button);

int code[] = {2,2,2,2,2,2}; //dummy code for security
                        //separated by commas

int entered[7]; //create a new empty array for the code entered by
                //the user (has 4 elements)
#define PIR_PIN     8    // Pin connected to the PIR sensor
#define BUTTON_PIN  3     // Pin connected to the button
#define LED_PIN     9     // Pin connected to the NeoPixel strip
#define LED_COUNT  10     // Number of LEDs in the strip

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int lastButtonState = LOW;  // Previous state of the button
int color = 0;       // Current color index
int pirState = LOW;         // Current state of the PIR sensor
int lastPirState = LOW;     // Previous state of the PIR sensor

uint32_t colors[] = {strip.Color(255, 0, 0), strip.Color(0, 0, 235), };
int numColors = 2; // Total number of colors

void setup() {
  Serial.begin(9600); //begin Serial
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  pinMode(button1, INPUT_PULLUP); //button 1 is an input
  pinMode(button3, INPUT_PULLUP); //button 3 is an input
  pinMode(button4, INPUT_PULLUP); //button 3 is an input
  pinMode(Red, OUTPUT); //the red LED is an output
  pinMode(greenLed, OUTPUT); // the green LED is an output
  digitalWrite(Red, LOW); //turn the red LED on
  for (int i = 0; i < 6;i++){ //work through numbers 0-3
    Serial.println(code[i]); //print each digit of the code
    Serial.println(entered[i]); //print each element of the entered[]
                                //array (this was for me to check that it 
                                //started at 0
  }
  strip.begin();                          // Initialize the NeoPixel strip
  strip.show();                           // Initialize all pixels to 'off'
  pinMode(PIR_PIN, INPUT);                // Initialize the PIR sensor pin as input
  pinMode(BUTTON_PIN, INPUT_PULLUP);      // Initialize the button pin as input with internal pull-up
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN); // Read the state of the button
  // Check if button state changed from LOW to HIGH (button press)
  if (buttonState == HIGH && lastButtonState == LOW) {
    color = (color + 1);
    if ((color%2) == 0) {
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Closed. ");
      lcd.setCursor(3,1);
      lcd.print("Passcode? ");
      strip.fill(strip.Color(255, 0, 0));
      strip.show();
    }
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Open! Press any ");
      lcd.setCursor(0,1);
      lcd.print("button to enter.");
      strip.fill(strip.Color(0, 0, 255));
      strip.show();
    }
  }
  // Update the last button state
  if ((color%2) == 0) {
    if (digitalRead(button1) == LOW){ //if button2 is pressed
      checkEntered1(2); //call checkEntered1 and pass it a 2
      delay(250); //wait
      }
    else if (digitalRead(button3) == LOW){ //if button3 is pressed
      checkEntered1(3); //call checkEntered1 and pass it a 3
      delay(250); //wait
          }
    else if (digitalRead(button4) == LOW){ //if button3 is pressed
      checkEntered1(4); //call checkEntered1 and pass it a 3
      delay(250); //wait
          }
  }
  else {
    if ((digitalRead(button1) == LOW) || (digitalRead(button3) == LOW) || (digitalRead(button4) == LOW) ){
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Wait... ");
      // motor pin
      delay(2000);
      lcd.clear();
      lcd.print("Enter! Push door.");
      for (int i = 0; i < 4; i++) {
        strip.fill(strip.Color(12, 255, 12));
        strip.show();
        delay(500);
        strip.fill(strip.Color(0, 0, 0));
        strip.show();
        delay(500);
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Open! Press any ");
      lcd.setCursor(0,1);
      lcd.print("button to enter.");
      strip.fill(strip.Color(0, 0, 255));
      strip.show();
      strip.fill(strip.Color(0, 0, 255));
      strip.show();
    }

  }
  lastButtonState = buttonState;
}

void checkEntered1(int button){ //check the first element of the entered[] array

  if (entered[0] != 0){ //if it is not a zero, i.e. it has already been inputted
    checkEntered2(button); //move on to checkEntered2, passing it "button"
  }
  
  else if(entered[0] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[0] = button; //set the first element as the button that has been pressed
    Serial.print("1: ");Serial.println(entered[0]); //for debugging
  }
  
}
void checkEntered2(int button){ //check the second element of the entered[] array

  if (entered[1] != 0){ //if it is not a zero, i.e. it has already been inputted
    checkEntered3(button); //move on to checkEntered3, passing it "button"
  }
  else if(entered[1] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[1] = button; //set the second element as the button that has been pressed
    Serial.print("2: ");Serial.println(entered[1]); //for debugging
  }
  
}
void checkEntered3(int button){  //check the third element of the entered[] array

  if (entered[2] != 0){ //if it is not a zero, i.e. it has already been inputted
    checkEntered4(button); //move on to checkEntered4, passing it "button"
  }
  
  else if (entered[2] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[2] = button; //set the third element as the button that has been pressed
    Serial.print("3: ");Serial.println(entered[2]); //for debugging
  }
  
}
void checkEntered4(int button){  //check the third element of the entered[] array

  if (entered[3] != 0){ //if it is not a zero, i.e. it has already been inputted
    checkEntered5(button); //move on to checkEntered4, passing it "button"
  }
  
  else if (entered[3] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[3] = button; //set the third element as the button that has been pressed
    Serial.print("4: ");Serial.println(entered[3]); //for debugging
  }
  
}
void checkEntered5(int button){  //check the third element of the entered[] array

  if (entered[4] != 0){ //if it is not a zero, i.e. it has already been inputted
    checkEntered6(button); //move on to checkEntered4, passing it "button"
  }
  
  else if (entered[4] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[4] = button; //set the third element as the button that has been pressed
    Serial.print("5: ");Serial.println(entered[4]); //for debugging
  }
  
}
void checkEntered6(int button){ //check the fourth element of the entered[] array
  if (entered[5] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[5] = button; //set the final element as the button that has been pressed
    Serial.print("6: ");Serial.println(entered[5]); //for debugging
    delay(100); //allow time for processing
    compareCode(); //call the compareCode function
  }
}

void compareCode(){ //checks if the code entered is correct by comparing the code[] array with the entered[] array
  for (int i = 0; i<6;i++){ //these three lines are for debugging
    Serial.println(entered[i]);
  }// right?
  if ((entered[0]==code[0]) && (entered[1]==code[1]) && (entered[2]==code[2]) && (entered[3]==code[3]) && (entered[4]==code[4])&& (entered[5]==code[5])){ //if all the elements of each array are equal
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Wait... ");
    // motor pin
    delay(2000);
    lcd.clear();
    lcd.print("Enter! Push door. ");
    for (int i = 0; i < 4; i++) {
      strip.fill(strip.Color(12, 255, 12));
      strip.show();
      delay(500);
      strip.fill(strip.Color(0, 0, 0));
      strip.show();
      delay(500);
      }
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Closed. ");
    lcd.setCursor(3,1);
    lcd.print("Passcode? ");
    for (int i = 0; i < 7; i++){ //this next loop is for debugging
      entered[i] = 0;
    }
    loop(); //return to loop() (not really necessary)
  }
  
  else {// wrong?
     //if you (or the intruder) get the code wrong
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Wrong code. ");
    for (int i = 0; i < 4; i++) {
      strip.fill(strip.Color(255, 0, 0));
      strip.show();
      delay(500);
      strip.fill(strip.Color(0, 0, 0));
      strip.show();
      delay(500);
      }
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Closed. ");
    lcd.setCursor(3,1);
    lcd.print("Passcode? ");
    
    strip.fill(strip.Color(255, 0, 0));
    strip.show();
    strip.fill(strip.Color(255, 0, 0));
    strip.show();
    for (int i = 0; i < 7; i++){ //this next loop is for debugging
      entered[i] = 0;
     
    }
    loop();
  }
}
