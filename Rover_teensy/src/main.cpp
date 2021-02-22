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
String Type = "";
String Data_A = "";
String Data_B = "";

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
void serialEvent();
void serialEvent1();
void serialEvent2();


void dirControl(int lSpeed, int rSpeed);
void dynamicControl(int throttle, int steering);
void mStop();

BTS7960 lMotorController(L_EN, L_L_PWM, L_R_PWM);
BTS7960 rMotorController(R_EN, R_L_PWM, R_R_PWM);
Nunchuk nchuk;

void setup() 
{
  Serial1.begin(9600); //From XBEE
  Serial2.begin(9600); //From NANO
  Serial.begin(9600); //From Desktop == Debug/program
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

	if (!success) {  // Lost conection with controller
		Serial1.println("Nunchuk  disconnected!");
		delay(1000);
	}
  else{
    zButton = nchuk.buttonZ();
    Steering = nchuk.joyX();
    Throttle = nchuk.joyY();
    Steering = map(Steering, 0, 256, maxNegSpeed, maxSpeed);
    Throttle = map(Throttle, 0, 256, maxNegSpeed, maxSpeed);

    Serial.print(Throttle);  
    Serial.print(" ");
    Serial.print(Steering);  
    Serial.print(" ");
    Serial.println(zButton);

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
    rMotorController.TurnRight(rSpeed);
  }
  else if (rSpeed < -deadZone){
    rSpeed = map(rSpeed, minSpeed, maxNegSpeed, minSpeed, maxSpeed);
    constrain(rSpeed, minSpeed, maxSpeed);
    rMotorController.TurnLeft(rSpeed);
  }
  else{rSpeed = 0;}

  if(lSpeed == 0 && rSpeed == 0){
    mStop();
  }
  
}

void dynamicControl(int throttle, int steering){
  int leftSpeed = throttle + steering; 
  int rightSpeed = throttle - steering; 
  dirControl(leftSpeed, rightSpeed);
}

void mStop(){
  lMotorController.Stop();
  lMotorController.Disable();
  rMotorController.Stop();
  rMotorController.Disable();
}

void serialEvent1() { //From XBEE
  while (Serial1.available()) {
    // add it to the inputString:
    Data_In = Serial1.readStringUntil('#');
    SerialParser(Data_In);
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    dataComplete = true;
  }
}
void serialEvent() { //From Desktop == Debug/program
  while (Serial.available()) {
    // add it to the inputString:
    Data_In = Serial.readStringUntil('#');
    //SerialParser(Data_In);
    //Serial1.println(Data_In);
    dataComplete = true;
  }
}
void serialEvent2() { //From NANO
  while (Serial2.available()) {
    // add it to the inputString:
    Data_In = Serial2.readStringUntil('#');
    SerialParser(Data_In);
    //Serial1.println(Data_In);
    dataComplete = true;
  }
}

void SerialParser(String Com) {
  Type = Com.substring(0, Com.indexOf("@"));
  Data_A = Com.substring(Com.indexOf("@") + 1, Com.indexOf("$"));
  Data_B = Com.substring(Com.indexOf("$") + 1, Com.indexOf("#"));
  //Serial1.println("Data parsed");
  // Serial1.println(Type);
  // Serial1.println(Data_A);
  // Serial1.println(Data_B);
  if (Type == "move"){
    dynamicControl(Data_A.toInt(), Data_B.toInt());
    Data_In = "";
    Type = "";
    Data_A = "";
    Data_B = "";
    dataComplete = false;

  }
}