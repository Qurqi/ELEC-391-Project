#include "Arduino_BMI270_BMM150.h"

int motorA_input1 = 2;
int motorA_input2 = 3;
int motorB_input1 = 4;
int motorB_input2 = 5;
int speed, speed_input;

float k = 0.8;
unsigned long start_time = 0;
unsigned long end_time = 0;
float G_angle_change;
float NG_angle_change;
float Ax, Ay, Az, Gx, Gy, Gz, A_angle = 0.0;
float W_angle = 0.0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IMU.begin(); 
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  delay(100);

  pinMode(motorA_input1, OUTPUT);
  pinMode(motorA_input2, OUTPUT);
  pinMode(motorB_input1, OUTPUT);
  pinMode(motorB_input2, OUTPUT);

  analogWrite(motorA_input1,0);
  analogWrite(motorA_input2,0);
  analogWrite(motorB_input1,0);
  analogWrite(motorB_input2,0);
  
}

void loop() {
  // check for user input 
      // if user input is 'k' send k value
      if (IMU.accelerationAvailable()) {
              IMU.readAcceleration(Ax, Ay, Az);
      }
      if (IMU.gyroscopeAvailable()) {
                IMU.readGyroscope(Gx, Gy, Gz);
                // read the sampling rate
                // calculate the angle
                // replace G_angle[0] with G_angle[1], then replace G_angle[1] with Gy
                start_time = micros();
                G_angle_change = Gx*(start_time-end_time)*0.000001;
                end_time =micros();
          }

      A_angle = atan(Ay/Az)*180/3.1415926;
      // calculate weighted angle
      // update previous angle reading
      NG_angle_change = -1*G_angle_change;
      Serial.println("-------------"); 
      Serial.println(NG_angle_change);
      Serial.println(A_angle);
      W_angle = k*(W_angle+NG_angle_change) + (1-k)*(A_angle);
      Serial.println(W_angle);
      Serial.println(k);
      speed = W_angle*255/20;
      speed_input = speed;
      if(speed < 0.0){
        speed_input = -speed;
      }
      motor_Control(speed_input, W_angle);

}

void motor_Control(int speed,float W_angle){
  if(W_angle >0.4){
  analogWrite(motorA_input1,speed); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorA_input2,0);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input1,speed); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input2,0);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  }
  else if(W_angle <-0.4){
  analogWrite(motorA_input1,0); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorA_input2,speed);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input1,0); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input2,speed);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  }
  else{
  analogWrite(motorA_input1,0); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorA_input2,0);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input1,0); // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  analogWrite(motorB_input2,0);   // 0 for 0%; 64 for 25%; 127 for 50%; 191 for 75%
  }
}
