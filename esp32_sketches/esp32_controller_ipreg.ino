/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>

const char* ssid = "BYLD";
const char* password = "";
String robotname = "BIGBOT";
const char * regserverIP = "10.44.32.249";
const int regserverPort = 11000;

std::string avalue;
WebServer server(80);
//The udp library class
WiFiUDP udp;

void handleRoot() {
  String message = "hello my name is: ";
  message += WiFi.localIP().toString();
  server.send(200, "text/plain", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

// Motor A

int pwmA = 13;
int in1A = 12;
int in2A = 27;

// Motor B

int pwmB = 14;
int in1B = 25;
int in2B = 26;
int Standby = 32;
// Speed control potentiometers

int SpeedControl1 = A0;  
int SpeedControl2 = A1;


// Motor Speed Values - Start at zero

int MotorSpeed1 = 0;
int MotorSpeed2 = 0;

// Setting PWM properties
const int freq = 30000;
const int pwmChannelA = 0;
const int pwmChannelB = 0;
const int resolution = 8;
int dutyCycle = 200;

#define STOP 0
#define FWD 1
#define BACK 2
#define LEFT 3
#define RIGHT 4
#define DELAY 5
#define SLOW 200
#define MEDIUM 225
#define FAST 255
int state = STOP;
int speedState = SLOW;
int carspeed = SLOW;
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

   // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  String msg = robotname + ":" + WiFi.localIP().toString();
  Serial.println(msg);
  uint8_t charbuffer[50];
  udp.beginPacket(regserverIP,regserverPort);
  msg.getBytes(charbuffer,50);
  udp.write(charbuffer, msg.length());
  Serial.println("Sent Msg");
  //udp.printf("Seconds since boot: %lu", millis()/1000);
  udp.endPacket();
  //Wait for 1 second
  delay(1000);

  udp.write(charbuffer, msg.length());
  Serial.println("Sent Msg");
  //udp.printf("Seconds since boot: %lu", millis()/1000);
  udp.endPacket();
  //Wait for 1 second
  delay(1000);

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/fwd", []() {
    server.send(200, "text/plain", "fwd");
    state = FWD;
    Serial.println("fwd");
  });

  server.on("/back", []() {
    server.send(200, "text/plain", "back");
    state = BACK;
    Serial.println("back");
  });

  server.on("/left", []() {
    server.send(200, "text/plain", "left");
    state = LEFT;
    Serial.println("left");
  });

  server.on("/right", []() {
    server.send(200, "text/plain", "right");
    state = RIGHT;
    Serial.println("right");
  });

  server.on("/stop", []() {
    server.send(200, "text/plain", "stop");
    state = STOP;
    Serial.println("stop");
  });

  server.on("/slow", []() {
    server.send(200, "text/plain", "stop");
    speedState = SLOW;
    Serial.println("slow");
  });

  server.on("/medium", []() {
    server.send(200, "text/plain", "stop");
    speedState = MEDIUM;
    Serial.println("medium");
  });

  server.on("/fast", []() {
    server.send(200, "text/plain", "stop");
    speedState = FAST;
    Serial.println("fast");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  pinMode(in1B, OUTPUT);
  pinMode(in2B, OUTPUT);
  pinMode(in2B, OUTPUT);
  pinMode(Standby, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannelA, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(pwmA, pwmChannelA);

    // configure LED PWM functionalitites
  ledcSetup(pwmChannelB, freq, resolution);

    // attach the channel to the GPIO to be controlled
  ledcAttachPin(pwmB, pwmChannelB);

  digitalWrite(Standby, LOW);
  Serial.println("Robot ready!");
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
bool notSeen = true;
void loop() {
  server.handleClient();
  String mystring(avalue.c_str()); 
  
  
switch(speedState){
  case SLOW:
    carspeed = SLOW;
    speedState=SLOW;
    break;
  case MEDIUM:
    carspeed = MEDIUM;
    speedState=MEDIUM;
    break;
  case FAST:
    carspeed = FAST;
    speedState=FAST;
    break;
  }
  
  switch(state){
    case FWD:
      forward(carspeed);
      state=DELAY;
      break;
    case BACK:
      reverse(carspeed);
      state=DELAY;
      break;
    case LEFT:
      left(carspeed);
      state=DELAY;
      break;
    case RIGHT:
      right(carspeed);
      state=DELAY;
      break;
    case STOP:
      brake();
      break;
    case DELAY:
      delay(1000);
      break;
  }
  
}
