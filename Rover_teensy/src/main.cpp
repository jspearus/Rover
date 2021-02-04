#include <Arduino.h>
#include "BTS7960.h"
#include <NintendoExtensionCtrl.h>

const uint8_t L_EN = 37;
const uint8_t L_L_PWM = 35;
const uint8_t L_R_PWM = 36;

const uint8_t R_EN = 23;
const uint8_t R_L_PWM = 21;
const uint8_t R_R_PWM = 22;

String Data_In = "";
String Dest = "";
String Datatype = "";
String Data = "";

int Throttle = 0;
int Steering =0;
int minSpeed = 0;
int maxSpeed = 100;
int maxNegSpeed = -100;
int deadZone= 5;
int speedOffset = 5;

boolean dataComplete = false;
boolean zButton = false;

void SerialParser(String Com);
void serialEvent1();

void dirControl(int lSpeed, int rSpeed);
void mForward(int speed, int time);
void mBackward(int speed, int time);
void mLeft(int speed, int time);
void mRight(int speed, int time);
void mStop();

BTS7960 lMotorController(L_EN, L_L_PWM, L_R_PWM);
BTS7960 rMotorController(R_EN, R_L_PWM, R_R_PWM);
Nunchuk nchuk;

void setup() 
{
  Serial1.begin(9600);
  Serial.begin(9600);
  nchuk.begin();
  delay(500);
  Serial1.println("Xbee Online!");
  while (!nchuk.connect()) {
		Serial1.println("Nunchuk not detected!");
		delay(1000);
	}

}

void loop() 
{
  boolean success = nchuk.update();  // Get new data from the controller

	if (!success) {  // Ruh roh
		Serial1.println("Controller disconnected!");
		delay(1000);
	}
  else{
    zButton = nchuk.buttonZ();
    Steering = nchuk.joyX();
    Throttle = nchuk.joyY();
    Steering = map(Steering, 0, 256, maxNegSpeed, maxSpeed);
    Throttle = map(Throttle, 0, 256, maxNegSpeed, maxSpeed);

    // Serial.print(Throttle);  //forward 255 back -255
    // Serial.print(" ");
    // Serial.print(Steering);  // left -255right 255
    // Serial.print(" ");
    // Serial.println(zButton);

    int leftSpeed = Throttle + Steering; 
    int rightSpeed = Throttle - Steering; 
    dirControl(leftSpeed, rightSpeed);

    if(zButton == true){
      while(zButton == true){
        mStop();
        nchuk.update(); 
        zButton = nchuk.buttonZ();
        delay(10);
      }
      
    }
  }
  delay(10);
}

void dirControl(int lSpeed, int rSpeed){
  //Direct control function
  lMotorController.Enable();
  rMotorController.Enable();

  constrain(lSpeed, maxNegSpeed, maxSpeed);
  constrain(rSpeed, maxNegSpeed, maxSpeed);

  if (lSpeed > deadZone){
    lMotorController.TurnRight(lSpeed + speedOffset);
  }
  else if (lSpeed < -deadZone){
    lSpeed = map(lSpeed, minSpeed, maxNegSpeed, minSpeed, maxSpeed);
    constrain(lSpeed, minSpeed, maxSpeed);
    lMotorController.TurnLeft(lSpeed + speedOffset);
  }
  else{lSpeed = 0;}

  if (rSpeed > deadZone){
    rMotorController.TurnLeft(rSpeed);
  }
  else if (rSpeed < -deadZone){
    rSpeed = map(rSpeed, minSpeed, maxNegSpeed, minSpeed, maxSpeed);
    constrain(rSpeed, minSpeed, maxSpeed);
    rMotorController.TurnRight(rSpeed);
  }
  else{rSpeed = 0;}

  if(lSpeed == 0 && rSpeed == 0){
    mStop();
  }
  
}

void mForward(int speed, int time){
  lMotorController.Enable();
  rMotorController.Enable();
   for(int i = 0 ; i < speed; i+=10)
  {
	lMotorController.TurnRight(i);
  rMotorController.TurnLeft(i);
	delay(100);
  }
  delay(time);
  mStop();
}

void mBackward(int speed, int time){
  //Serial.println("back");
  lMotorController.Enable();
  rMotorController.Enable();
   for(int i = 0 ; i < speed; i+=10)
  {
	rMotorController.TurnRight(i);
  lMotorController.TurnLeft(i);
	delay(100);
  }
  delay(time);
  mStop();
}

void mLeft(int speed, int time){
  //Serial.println("Left");
  lMotorController.Enable();
  rMotorController.Enable();
   for(int i = 0 ; i < speed; i+=10)
  {
	rMotorController.TurnLeft(i);
  lMotorController.TurnLeft(i);
	delay(100);
  }
  delay(time);
  mStop();
}

void mRight(int speed, int time){
  //Serial.println("Right");
  lMotorController.Enable();
  rMotorController.Enable();
   for(int i = 0 ; i < speed; i+=10)
  {
	rMotorController.TurnRight(i);
  lMotorController.TurnRight(i);
	delay(100);
  }
  delay(time);
  mStop();
}

void mStop(){
  lMotorController.Stop();
  lMotorController.Disable();
  rMotorController.Stop();
  rMotorController.Disable();
}

void serialEvent1() { //Debug Comm
  while (Serial1.available()) {
    // add it to the inputString:
    Data_In = Serial1.readStringUntil('#');
    SerialParser(Data_In);
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    dataComplete = true;
  }
}

void SerialParser(String Com) {
  Dest = Com.substring(0, Com.indexOf("@"));
  Datatype = Com.substring(Com.indexOf("@") + 1, Com.indexOf("-"));
  Data = Com.substring(Com.indexOf("-") + 1, Com.indexOf("#"));
  // Serial1.println("Data parsed");
  // Serial1.println(Dest);
  // Serial1.println(Datatype);
  // Serial1.println(Data);
  if (Dest == "rover"){
    if (Datatype == "move"){
      if (Data == "f"){
        Serial1.println("Moving Forward");
        mForward(75, 500);
      }
      else if (Data == "b"){
        Serial1.println("Moving Backward");
        mBackward(75, 500);
      }
      else if (Data == "l"){
        Serial1.println("Moving Left");
        mLeft(75, 500);
      }
      else if (Data == "r"){
        Serial1.println("Moving Right");
        mRight(75, 500);
      }
    }
    Data_In = "";
    Dest = "";
    Datatype = "";
    Data = "";
  }
}