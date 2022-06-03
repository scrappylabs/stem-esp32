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

int sensorA = 36;
int sensorB = 39;

unsigned long start_time = 0;
unsigned long end_time = 0;
float temp=0;
int stepsA=0;
float steps_oldA=0;
float rpsA=0;

int stepsB=0;
float steps_oldB=0;
float rpsB=0;

int delta=1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Starts the serial communication
  pinMode(sensorA,INPUT_PULLUP);
  pinMode(sensorB,INPUT_PULLUP);
  //Serial.print("A-STEPS - 0");
  //Serial.print("A-RPS - 0");
  //Serial.print("B-STEPS - 0");
  //Serial.print("B-RPS - 0");
  
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
  // put your main code here, to run repeatedly:

  // Set Motor A forward
  digitalWrite(in1A, HIGH);
  digitalWrite(in2A, LOW);

 // Set Motor B forward
  digitalWrite(in1B, HIGH);
  digitalWrite(in2B, LOW);
  digitalWrite(Standby, HIGH);

  if(dutyCycle<=200){
    delta=50;
  }
  if(dutyCycle >= 255){
    delta=-50;
  }

  dutyCycle+=delta;
  ledcWrite(pwmChannelA, dutyCycle);
  ledcWrite(pwmChannelB, dutyCycle);
  
  
start_time=millis();
end_time=start_time+1000;
while(millis()<end_time)
{
   if(digitalRead(sensorA))
   {
    while(digitalRead(sensorB));
    stepsA=stepsA+1;
    //while(digitalRead(sensorA));
   }
   if(digitalRead(sensorB)){
    while(digitalRead(sensorA));
    stepsB=stepsB+1;
    //while(digitalRead(sensorB));
   }

   //Serial.print("A-STEPS: ");
   //Serial.println(stepsA);
   //Serial.print("B-STEPS: ");
   //Serial.println(stepsB);
}
   temp=stepsA-steps_oldA;
   steps_oldA=stepsA;
   rpsA=(temp/20);
   Serial.print("A-RPS: ");
   Serial.println(rpsA);
   temp=stepsB-steps_oldB;
   steps_oldB=stepsB;
   rpsB=(temp/20);
   //Serial.print("B-RPS: ");
   //Serial.println(rpsB);
  
  delay(5000);
  
}
