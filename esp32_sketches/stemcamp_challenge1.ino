/*
  This sketch provides functions to move the robot car Left, Right, Foward, and Backward
*/

// Motor A

int pwmA = 13;
int in1A = 12;
int in2A = 27;

// Motor B

int pwmB = 14;
int in1B = 25;
int in2B = 26;
int Standby = 32;

int SpeedControl1 = A0;  
int SpeedControl2 = A1;

// Setting PWM properties
const int freq = 30000;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
const int resolution = 8;
int dutyCycle = 200; // Speed of the motors should only be between 200 and 255

void setup() {
  Serial.begin(115200);
  // Set all the motor control pins to outputs
  pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  pinMode(in1B, OUTPUT);
  pinMode(in2B, OUTPUT);
  pinMode(in2B, OUTPUT);
  pinMode(Standby, OUTPUT);
  
  // configure PWMA functionalitity using ledc
  ledcSetup(pwmChannelA, freq, resolution);
  
  // attach Channel A to Motor A (left motor)
  ledcAttachPin(pwmA, pwmChannelA);

  // configure PWMB functionalitity using ledc
  ledcSetup(pwmChannelB, freq, resolution);

  // attach Channel B to Motor B (right motor)
  ledcAttachPin(pwmB, pwmChannelB);

  digitalWrite(Standby, LOW);
}

void forward(int dutyCycle) {                  
 digitalWrite(Standby, HIGH);
 digitalWrite(in1A, HIGH);
 digitalWrite(in2A, LOW);   
 digitalWrite(in1B, HIGH);
 digitalWrite(in2B, LOW);     
 ledcWrite(pwmChannelA, dutyCycle);
 ledcWrite(pwmChannelB, dutyCycle);               
}

void reverse(int dutyCycle) {       
 digitalWrite(Standby, HIGH);           
 digitalWrite(in1A, LOW);
 digitalWrite(in2A, HIGH);   
 digitalWrite(in1B, LOW);
 digitalWrite(in2B, HIGH);     
 ledcWrite(pwmChannelA, dutyCycle);
 ledcWrite(pwmChannelB, dutyCycle);               
}

void brake(){
 digitalWrite(in1A, HIGH);
 digitalWrite(in2A, HIGH);   
 digitalWrite(in1B, HIGH);
 digitalWrite(in2B, HIGH); 
 ledcWrite(pwmChannelA, 0);
 ledcWrite(pwmChannelB, 0); 
 digitalWrite(Standby, LOW);
}

void left(int dutyCycle)
{
  digitalWrite(Standby, HIGH);
  digitalWrite(in1A, LOW);
  digitalWrite(in2A, HIGH);
  ledcWrite(pwmChannelA, dutyCycle);
  
  digitalWrite(in1B, HIGH);
  digitalWrite(in2B, LOW);
  ledcWrite(pwmChannelB, dutyCycle);
}

void right(int dutyCycle)
{
  digitalWrite(Standby, HIGH);
  digitalWrite(in1A, HIGH);
  digitalWrite(in2A, LOW);
  ledcWrite(pwmChannelA, dutyCycle);
  
  digitalWrite(in1B, LOW);
  digitalWrite(in2B, HIGH);
  ledcWrite(pwmChannelB, dutyCycle);
}

void loop() {
  forward(200);
  delay(500);
  
  brake();
  delay(500);
  
  reverse(200);
  delay(500);
  
  brake();
  delay(500);
  
  left(200);
  delay(500);
  
  right(200);
  delay(500);
  
  brake();
  delay(500);
}
