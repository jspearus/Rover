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

boolean dataComplete = false;

void SerialParser(String Com);
void serialEvent1();

void dirControl(int lSpeed, int rSpeed, int time);
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
    boolean zButton = nchuk.buttonZ();
    Steering = nchuk.joyX();
    Throttle = nchuk.joyY();
    Steering = map(Steering, 0, 256, -100, 100);
    Throttle = map(Throttle, 0, 256, -100, 100);

    // Serial.print(Throttle);  //forward 255 back -255
    // Serial.print(" ");
    // Serial.print(Steering);  // left -255right 255
    // Serial.print(" ");
    // Serial.println(zButton);

    int leftSpeed = Throttle + Steering; 
    int rightSpeed = Throttle - Steering; 
    dirControl(leftSpeed, rightSpeed, 0);
    if(zButton == true){
      mStop();
    }
  }
  delay(10);
}

void dirControl(int lSpeed, int rSpeed, int time = 0){
  //Direct control function
  lMotorController.Enable();
  rMotorController.Enable();
  constrain(lSpeed, -100, 100);
  constrain(rSpeed, -100, 100);
  if (lSpeed > 5){
    lMotorController.TurnRight(lSpeed);
  }
  else if (lSpeed < -5){
    lSpeed = map(lSpeed, 0, -100, 0, 100);
    constrain(lSpeed, 0, 100);
    lMotorController.TurnLeft(lSpeed);
  }
  else{lSpeed = 0;}
  if (rSpeed > 5){
    rMotorController.TurnLeft(rSpeed);
  }
  else if (rSpeed < -5){
    rSpeed = map(rSpeed, 0, -100, 0, 100);
    constrain(rSpeed, 0, 100);
    rMotorController.TurnRight(rSpeed);
  }
  else{rSpeed = 0;}

  if(lSpeed == 0 && rSpeed == 0){
    mStop();
  }
  if(time > 0){
    delay(time);
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