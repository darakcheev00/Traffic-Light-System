/*
Daniel Arakcheev
Ganesh Krishna
Michael Wu

TRAFFIC LIGHT SYSTEM
6/4/2017
TEJ 3U3
Mr. Wong
*/

//Servo motor library
#include <Servo.h>

Servo SERVO_ARM;

//-------------------------------------------------------------------------------
// Initialization of constant variables which hold the pin number of the arduino
//-------------------------------------------------------------------------------
const int SINGLE_RED_LIGHT = 1;
const int SINGLE_YELLOW_LIGHT = 4;
const int SINGLE_GREEN_LIGHT = 7;

const int DOUBLE_RED_LIGHT = 0;
const int DOUBLE_YELLOW_LIGHT = 3;
const int DOUBLE_GREEN_LIGHT = 6;

const int PEDESTRIAN_RED_LIGHT = 10;
const int PEDESTRIAN_WHITE_LIGHT = 12;

const int STREET_LIGHT = 8;  
const int BUTTON_INPUT = 13;

const int LDR_SENSOR = A4;
const int IR_SENSOR = A5;

// The minimum reading of IR sensor to activate gate
const int IR_MIN = 50; 

// The min LDR reading to turn on street light
const int LDR_MIN = 500;

// The max LDR reading to turn off street light
const int LDR_MAX = 600;

// Timer variable used to multitask (ticks)
int t1 = 0;

// Timer used to hold gate open
int t2 = 0;

// Default time until pedestrian light (in timer ticks)
int timeToPedestrian = 110;

// Boolean used to determine if street light is on
boolean isStreetLightOn = false;

// Boolean used to determine if gate openned
boolean isOpened = false;

// Turns on a specific LED
void turnOn (int LED){
  digitalWrite (LED, HIGH);
}

// Turns off a specific LED
void turnOff(int LED){
  digitalWrite (LED, LOW);  
}

// Returns the state of the button
int getButton(){
  return  digitalRead(BUTTON_INPUT);
}

// Opens gate
void openGate(){
    SERVO_ARM.write(90);
}

// Closes gate
void closeGate(){
    SERVO_ARM.write(-90);
}

// Returns the voltage as an integer (between 0 and 1023) of the LDR sensor 
int getLDR (){
  return analogRead(LDR_SENSOR); 
}

// Returns the IR sensor voltage (on the next resistor) as integer (between 0 and 1023)
int getIR(){
  return analogRead(IR_SENSOR);  
}

// Turns off the first LED and turns on the second LED (as passed in the parameters)
void switchTo (int LED1, int LED2) {
    turnOff(LED1);
    turnOn(LED2);
}
//--------------------------------------------------------------
// The method that sets up all the pins to its correct function
//--------------------------------------------------------------
void setup() {
    pinMode(SINGLE_RED_LIGHT, OUTPUT);
    pinMode(SINGLE_YELLOW_LIGHT, OUTPUT);
    pinMode(SINGLE_GREEN_LIGHT, OUTPUT);
    pinMode(DOUBLE_RED_LIGHT, OUTPUT);
    pinMode(DOUBLE_YELLOW_LIGHT, OUTPUT);
    pinMode(DOUBLE_GREEN_LIGHT, OUTPUT);
    pinMode(PEDESTRIAN_RED_LIGHT, OUTPUT);
    pinMode(PEDESTRIAN_WHITE_LIGHT, OUTPUT);
    pinMode(STREET_LIGHT,OUTPUT);
    pinMode(IR_SENSOR,INPUT);
    pinMode(LDR_SENSOR,INPUT);
    pinMode(BUTTON_INPUT,INPUT);
    SERVO_ARM.attach(9);
    SERVO_ARM.write(0);
}

//----------------------------
// Main method that is looped
//----------------------------
void loop(){
  // Traffic & pedestrian lights
  if (t1 == 0){
    turnOn(SINGLE_RED_LIGHT);
    turnOn(DOUBLE_GREEN_LIGHT);
    turnOn(PEDESTRIAN_WHITE_LIGHT);
  }
  if (t1 == 50){
    switchTo(DOUBLE_GREEN_LIGHT, DOUBLE_YELLOW_LIGHT);
    switchTo(PEDESTRIAN_WHITE_LIGHT, PEDESTRIAN_RED_LIGHT);
  } 
  // If button is pressed during blinking red pedestrian light or traffic yellow light 
  if (t1>50 && t1<60) {
     if (getButton() == HIGH) {
        turnOff(PEDESTRIAN_RED_LIGHT);
        turnOff(DOUBLE_YELLOW_LIGHT);
        t1 = 0;
        return;
     }
  }
  // Blinking of the red pedestrian light
  if (t1 == 52){
    turnOff(PEDESTRIAN_RED_LIGHT);
  } 
  if (t1 == 54){
    turnOn(PEDESTRIAN_RED_LIGHT);
  } 
  if (t1 ==  56){
    turnOff(PEDESTRIAN_RED_LIGHT);
  } 
  if (t1 == 58){
    turnOn(PEDESTRIAN_RED_LIGHT);
  }
  if (t1 == 60){
    switchTo(DOUBLE_YELLOW_LIGHT, DOUBLE_RED_LIGHT);
    switchTo(SINGLE_RED_LIGHT, SINGLE_GREEN_LIGHT);
    timeToPedestrian = 110;
  }
  // Button action executed
  if (t1 > 60 && t1 < 110 && timeToPedestrian == 110){
    if (getButton() == HIGH) {
       timeToPedestrian = t1;
    }
  }
  if (t1 == timeToPedestrian){
    switchTo(SINGLE_GREEN_LIGHT, SINGLE_YELLOW_LIGHT);
  }
  if (t1 == timeToPedestrian + 10){
    switchTo(SINGLE_YELLOW_LIGHT, SINGLE_RED_LIGHT);
    switchTo(DOUBLE_RED_LIGHT, DOUBLE_GREEN_LIGHT);
    switchTo(PEDESTRIAN_RED_LIGHT,PEDESTRIAN_WHITE_LIGHT);
    // Resets main tick timer
    t1 = 0;
  }
  
  // IR Gate
  if (getIR() < IR_MIN){
    openGate();
    isOpened = true;
  }
  else 
  {
    if (isOpened) {
      // Starts gate timer
      t2++;
      // Waits 3 seconds after object has left the sensor
      if(t2 == 30){
        closeGate();
        isOpened = false;
        // Resets gate timer
        t2 = 0;
      }
    }
  }
  
  // Street light
  if (getLDR() < LDR_MIN){
    if (!isStreetLightOn){
      turnOn (STREET_LIGHT);
      isStreetLightOn = true;
    }
  }
  else if (getLDR() > LDR_MAX){
    if (isStreetLightOn){
      turnOff(STREET_LIGHT);
      isStreetLightOn = false;
    }
  }

  // Timer tick increment
  t1++;
  // Delay between ticks (0.1 seconds)
  delay(100);
}
  
  
  
  
  
