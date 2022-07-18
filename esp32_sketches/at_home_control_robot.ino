#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
int slow_speed = 150;
int medium_speed = 200;
int fast_speed = 255;

// Wi-Fi 2.4GHz ONLY
// Do not upload to ESP32 without first setting these two credentials!
const char* ssid = "...";
const char* password = "...";
int carspeed = medium_speed; //slow_speed, medium_speed, or fast_speed

WebServer server(80);

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
const int pwmChannelB = 0;
const int resolution = 8;
int dutyCycle = 200;

#define STOP 0
#define FWD 1
#define BACK 2
#define LEFT 3
#define RIGHT 4
#define DELAY 5
#define SLOW_STATE 6
#define MEDIUM_STATE 7
#define FAST_STATE 8

int state = STOP;
String ptr = "";

void handleRoot() {
  server.send(200, "text/html", ptr);
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

void setup(void) {
  ptr = "<!DOCTYPE html>";
  ptr += "<head>";
  ptr += "<meta charset=\"utf-8\">";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  ptr += "<title>STEM CAMP</title>";
  ptr += "<style>";
  ptr += "* {box-sizing: border-box;}body {margin: 0;}.column {float: left;width: 33.33%;padding: 15px;margin-left: 60px;}";
  ptr += ".row:after {content: \"\";display: table;clear: both;}@media screen and (max-width:600px) { .column {width: 100%;}}";
  ptr += ".mid{display:inline-flex;}h1 {font-size: 80px;}h2{text-align: center;}p {font-size: 64px;}.stopbutton{width: 96%;margin-top: 10px;}";
  ptr += ".buttonM {font-family:\"Comic Sans MS\";font-size:20px;font-weight:bold;line-height:28px;text-align:center;color:\"0a0a0a;background-color:\"ffe100;padding:18px;}";
  ptr += ".buttonM:focus {background-color:white;}.smallbutton {font-family:\"Comic Sans MS\";font-size:20px;font-weight:bold;line-height:20px;text-align:center;color:\"0a0a0a;background-color:\"ffe100;padding:20px;width:117px;}";
  ptr += ".smallbutton:focus {background-color:white;}.bigfont{font-size: 30px;}";
  ptr += "</style>"; 
  ptr += "</head>";
  ptr += "<body>";
  ptr += "<div class=\"row\">";
  /*ptr += "<div class=\"column\">";
  ptr += "<table>";
  ptr += "<tbody>";
  ptr += "<tr>";
  ptr += "<td><a href=\"/fast\"><button class=\"smallbutton\" >fast</button></a></td>";
  ptr += "</tr>";
  ptr += "<tr>";
  ptr += "<td><a href=\"/medium\"><button class=\"smallbutton\" >medium</button></a></td>";
  ptr += "</tr>";
  ptr += "<tr>";
  ptr += "<td><a href=\"/slow\"><button class=\"smallbutton\" >slow</button></a></td>";
  ptr += "</tr>";
  ptr += "</tbody>";
  ptr += "</table>";
  ptr += "</div>";*/
  ptr += "<div class=\"column\">";
  ptr += "<table>";
  ptr += "<tbody>";
  ptr += "<tr>";
  ptr += "<td>&nbsp;</td>";
  ptr += "<td><a href=\"/fwd\"><button class=\"buttonM\" >FWD</button></a></td>";
  ptr += "<td>&nbsp;</td>";
  ptr += "</tr>";
  ptr += "<tr>";
  ptr += "<td><a href=\"/left\"><button class=\"buttonM\" >LEFT</button></a></td>";
  ptr += "<td><a href=\"/stop\"><button class=\"buttonM\" >STOP</button></a></td>";
  ptr += "<td><a href=\"/right\"><button class=\"buttonM\" >RIGHT</button></a></td>";
  ptr += "</tr>";
  ptr += "<tr>";
  ptr += "<td>&nbsp;</td>";
  ptr += "<td><a href=\"/back\"><button class=\"buttonM\" >BACK</button></a></td>";
  ptr += "<td>&nbsp;</td>";
  ptr += "</tr>";
  ptr += "</tbody>";
  ptr += "</table>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "<div class=\"row\">";
  ptr += "<div class=\"column\">";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</body></html>";

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

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/fwd", []() {
    server.send(200, "text/html", ptr);
    state = FWD;
  });

  server.on("/back", []() {
    server.send(200, "text/html", ptr);
    state = BACK;
  });

  server.on("/stop", []() {
    server.send(200, "text/html", ptr);
    state = STOP;
  });

  server.on("/left", []() {
    server.send(200, "text/html", ptr);
    state = LEFT;
  });

  server.on("/right", []() {
    server.send(200, "text/html", ptr);
    state = RIGHT;
  });

  /*server.on("/slow", []() {
    server.send(200, "text/html", ptr);
    carspeed = slow_speed;
    speedState = SLOW_STATE;
  });

  server.on("/medium", []() {
    server.send(200, "text/html", ptr);
    carspeed = medium_speed;
    speedState = MEDIUM_STATE;
  });

  server.on("/fast", []() {
    server.send(200, "text/html", ptr);
    carspeed = fast_speed;
    speedState = FAST_STATE;
  });*/

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  pinMode(in1B, OUTPUT);
  pinMode(in2B, OUTPUT);
  pinMode(in2B, OUTPUT);
  pinMode(Standby, OUTPUT);
  
  // configure Motor A PWM functionalitites
  ledcSetup(pwmChannelA, freq, resolution);
  
  // attach the channel to the GPIO to be controlled for Motor A
  ledcAttachPin(pwmA, pwmChannelA);

    // configure LED PWM functionalitites
  ledcSetup(pwmChannelB, freq, resolution);

    // attach the channel to the GPIO to be controlled for Motor B
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
  delay(500);
}

void reverse(int dutyCycle) {       
  digitalWrite(Standby, HIGH);           
  digitalWrite(in1A, LOW);
  digitalWrite(in2A, HIGH);   
  digitalWrite(in1B, LOW);
  digitalWrite(in2B, HIGH);     
  ledcWrite(pwmChannelA, dutyCycle);
  ledcWrite(pwmChannelB, dutyCycle);               
  delay(500);
}

void brake(){
  digitalWrite(in1A, HIGH);
  digitalWrite(in2A, HIGH);   
  digitalWrite(in1B, HIGH);
  digitalWrite(in2B, HIGH); 
  ledcWrite(pwmChannelA, 0);
  ledcWrite(pwmChannelB, 0); 
  delay(500);
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
  delay(500);
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
  delay(500);
}

void loop(void) {
  while(WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, password);
    delay(500);
    Serial.print(".");
  }

  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
  
  /*switch(speedState){
    case SLOW_STATE:
      carspeed = slow_speed;
      break;
    case MEDIUM_STATE:
      carspeed = medium_speed;
      state=DELAY;
      break;
    case FAST_STATE:
      carspeed = fast_speed;
      state=DELAY;
      break;
  }*/
  
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
