#include "Arduino_BMI270_BMM150.h"

char buffer[20];
char userInput;
float k = 0.50;
float W_angle[2] = {0,0};
float G_angle = 0;
float NG_angle = 0;
float rate;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  IMU.begin(); 
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  rate = IMU.gyroscopeSampleRate();
  Serial.println(rate);
  rate = (1/rate);
  Serial.println(rate);

  
}
void loop() {

  float Ax, Ay, Az, Gx, Gy, Gz, A_angle;

  // check for user input 

      userInput = Serial.read();
      // if user input is 'k' send k value

      if (IMU.gyroscopeAvailable()) {
                IMU.readGyroscope(Gx, Gy, Gz);
                // read the sampling rate
                // calculate the angle
                // replace G_angle[0] with G_angle[1], then replace G_angle[1] with Gy
                G_angle = G_angle + Gx*(rate);
          }

      if (IMU.accelerationAvailable()) {
              IMU.readAcceleration(Ax, Ay, Az);
              A_angle = atan(Ay/Az)*180/3.1415926;
      }
      // calculate weighted angle
      // update previous angle reading
      NG_angle = G_angle*-1;
      W_angle[1] = k*(W_angle[0]+NG_angle) + (1-k)*(A_angle);


      if (userInput == 'k') {
        if (Serial.available() > 0) {
          Serial.println(k);
        }
      }
    
      // if user input is 'd' return angle data
      if (userInput == 'd') {
          // pack the buffer with colon-delimited angle values
          sprintf(buffer, "%.2f:%.2f:%.2f", A_angle, NG_angle, W_angle[1]);
          // send the buffer to the serial port
          if (Serial.available() > 0) {
          Serial.println(buffer);
          //W_angle[0] = W_angle[1];
          }
          // clear the buffer 
          memset(buffer, 0, sizeof(buffer));
      }
      // if user input is 'n' iterate k-value by 0.01
      if (userInput == 'n') {
          k += 0.01;
          
      }
      // if user input is 'e' exit
      if (userInput == 'e'){
        if (Serial.available() > 0) {
        Serial.println("Exit signal Recieved. Transmission ended");
        }
        while (1);
      }
  
  
}
