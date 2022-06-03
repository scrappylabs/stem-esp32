/*
  TB6612FNG H-Bridge Demo
  This demo will move the car forward with increasing and decreasing speed.
  This demonstrates the PWM signals changing the speed of rotation.
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

// Setting PWM properties
const int freq = 30000;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
const int resolution = 8;
int dutyCycle = 200; // Motor Speed should be between 200 and 255

void setup(){
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
   
}

void loop() {
  
  // Set Motor A forward
  digitalWrite(in1A, HIGH);
  digitalWrite(in2A, LOW);

 // Set Motor B forward
  digitalWrite(in1B, HIGH);
  digitalWrite(in2B, LOW);
  digitalWrite(Standby, HIGH);

  // Move the car forward with increasing speed
  while (dutyCycle <= 254){
    ledcWrite(pwmChannelA, dutyCycle);
    ledcWrite(pwmChannelB, dutyCycle);
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }

  // Move the car forward with decreasing speed
  while (dutyCycle > 201){
    ledcWrite(pwmChannelA, dutyCycle);
    ledcWrite(pwmChannelB, dutyCycle);
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle - 5;
    delay(500);
  }
  
}
