#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor3 = AFMS.getMotor(3);
Adafruit_DCMotor *myMotor4 = AFMS.getMotor(4);

// Pin definitions
int SEL_BUTT = 2;
int UP_BUTT = 3;
int DOWN_BUTT = 4;

// Global variables
int firstIndex = 0;
double customRecipe[] = {0.3, 0 , 0, 0};
int master[] = {customRecipe};
int ml = sizeof(master)/sizeof(master[0]);


double syrupTime1 = 7000;
double syrupTime2 = 7000;
double syrupTime3 = 7000;
double halfTime = 3500;

int prevSelectButton = LOW;

void setup() {
  pinMode(SEL_BUTT, INPUT_PULLUP);
  pinMode(UP_BUTT, INPUT_PULLUP);
  pinMode(DOWN_BUTT, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  whichMode();
}

void whichMode() {
  
  int selectButton = digitalRead(SEL_BUTT);
  
  if (selectButton == HIGH && prevSelectButton == LOW) {
    firstIndex++;
  }
  if ((firstIndex%2) == 0) {
    presetMode();
  }
  else {
    customizeMode();
  }
  prevSelectButton = selectButton;
}

void presetMode() {
  bool go = true; 
  int safeIndex;
  int recipeIndex = 0;
  int prevSelectButton = LOW; // Used to detect changes in button state
  unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
  unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers

  Serial.println("Preset Mode Active");

  while (go) {
    int selectButton = digitalRead(SEL_BUTT);
    int upButton = digitalRead(UP_BUTT);
    int downButton = digitalRead(DOWN_BUTT);
    

    // Debounce logic for selectButton
    if (selectButton == LOW && prevSelectButton == LOW && (millis() - lastDebounceTime) > debounceDelay) {
      go = false; // Exit the loop
      lastDebounceTime = millis();
    }

    // Debounce logic for upButton
    if (upButton == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
      recipeIndex++;
      lastDebounceTime = millis(); // Reset debounce timer
    }

    // Debounce logic for downButton
    if (downButton == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
      recipeIndex--;
      lastDebounceTime = millis(); // Reset debounce timer
    }

    // Ensuring recipeIndex wraps around correctly
    safeIndex = (recipeIndex % ml + ml) % ml; 
    Serial.print("RI: ");
    Serial.println(master[safeIndex]);

    prevSelectButton = selectButton;
    delay(100); 

  }
  Serial.println("While loop done");
  dispensePreset(master[safeIndex]);
}


void customizeMode() {
  Serial.println("Custom");
}

void dispensePreset(double recipe[4]) {
  Serial.println("DISPENSE");
  delay(1000);

  Serial.println(syrupTime1 * recipe[0]);
  delay(1000);

  myMotor1->setSpeed(200);
  myMotor2->setSpeed(200);
  myMotor3->setSpeed(200);
  myMotor4->setSpeed(200);

  myMotor1->run(FORWARD);
  Serial.println(recipe[0]);
  delay(syrupTime1 * recipe[0]); 
  myMotor1->run(RELEASE);

  myMotor2->run(FORWARD);
  Serial.println(recipe[1]);
  delay(syrupTime2 * recipe[1]);
  myMotor2->run(RELEASE);

  myMotor3->run(FORWARD);
  Serial.println(recipe[2]);
  delay(syrupTime3 * recipe[2]);
  myMotor3->run(RELEASE);

  myMotor4->run(FORWARD);
  Serial.println(recipe[3]);
  delay(halfTime * recipe[3]);
  myMotor4->run(RELEASE);
}
