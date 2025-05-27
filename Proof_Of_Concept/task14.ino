#include "Arduino_BMI270_BMM150.h"

int motorA_input1 = 2;
int motorA_input2 = 3;
int motorB_input1 = 4;
int motorB_input2 = 5;
int speed;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IMU.begin(); 
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  pinMode(motorA_input1, OUTPUT);
  pinMode(motorA_input2, OUTPUT);
  pinMode(motorB_input1, OUTPUT);
  pinMode(motorB_input2, OUTPUT);
}

void loop() {
  // forward with four different speed
  forward(64);
  Serial.println("ok1");
  delay(3000);
  Serial.println("ok2");
  forward(127);
  Serial.println("ok2");
  delay(3000);
  forward(191);
  delay(3000);
  forward(255);
  delay(3000);
  stop();
  delay(1000);
// backward with four different speed
  backward(64);
  delay(3000);
  backward(191);
  delay(3000);
  stop();
  delay(1000);
// Reverse with four different speed
  reverse_one(64);
  delay(3000);
  reverse_one(191);
  delay(3000);
  stop();
  delay(1000);
// Another reverse with four different speed
  reverse_two(64);
  delay(3000);
  reverse_two(191);
  delay(3000);
  stop();
  delay(1000);
}


void forward(int speed){
  analogWrite(motorA_input1,speed); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorA_input2,0);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input1,speed); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input2,0);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
}

void backward(int speed){
  analogWrite(motorA_input1,0); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorA_input2,speed);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input1,0); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input2,speed);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
}

void reverse_one(int speed){
  analogWrite(motorA_input1,0); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorA_input2,speed);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input1,speed); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input2,0);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
}

void reverse_two(int speed){
  analogWrite(motorA_input1,speed); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorA_input2,0);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input1,0); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input2,speed);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
}

void stop(){
  analogWrite(motorA_input1,0); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorA_input2,0);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input1,0); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input2,0);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
}

