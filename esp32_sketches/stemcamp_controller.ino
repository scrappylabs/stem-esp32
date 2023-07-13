/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "stemcamp";
const char* password = "stemcamp2023";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
//String output26State = "off";
//String output27State = "off";

// Assign output variables to GPIO pins
//const int output26 = 26;
//const int output27 = 27;


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
  /*// Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
*/
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();

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

void loop(){

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
  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /fwd") >= 0) {
              Serial.println("FORWARD");
              state = FWD;
              //output26State = "on";
              //digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /back") >= 0) {
              Serial.println("BACKWARD");
              state = BACK;
              //output26State = "off";
              //digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /stop") >= 0) {
              Serial.println("STOP");
              state = STOP;
              //output27State = "on";
              //digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /left") >= 0) {
              Serial.println("LEFT");
              state = LEFT;
              //output27State = "off";
              //digitalWrite(output27, LOW);
            } else if (header.indexOf("GET /right") >= 0) {
              Serial.println("RIGHT");
              state = RIGHT;
              //output27State = "off";
              //digitalWrite(output27, LOW);
            } else if (header.indexOf("GET /slow") >= 0) {
              Serial.println("SLOW");
              state = SLOW;
              //output27State = "off";
              //digitalWrite(output27, LOW);
            }
            else if (header.indexOf("GET /medium") >= 0) {
              Serial.println("MEDIUM");
              state = MEDIUM;
              //output27State = "off";
              //digitalWrite(output27, LOW);
            }
            else if (header.indexOf("GET /fast") >= 0) {
              Serial.println("FAST");
              state = FAST;
              //output27State = "off";
              //digitalWrite(output27, LOW);
            }

            // Display the HTML web page

            client.println("<!DOCTYPE html>");
client.println("<head>");
client.println("<meta charset=\"utf-8\">");
    client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<title>STEM CAMP</title>");
    client.println("<style>");
    client.println("* {box-sizing: border-box;}body {margin: 0;}.column {float: left;width: 33.33%;padding: 15px;margin-left: 60px;}");
    client.println(".row:after {content: \"\";display: table;clear: both;}@media screen and (max-width:600px) { .column {width: 100%;}}");
    client.println(".mid{display:inline-flex;}h1 {font-size: 80px;}h2{text-align: center;}p {font-size: 64px;}.stopbutton{width: 96%;margin-top: 10px;}");
    client.println(".buttonM {font-family:\"Comic Sans MS\";font-size:20px;font-weight:bold;line-height:28px;text-align:center;color:#0a0a0a;background-color:#ffe100;padding:18px;}");
    client.println(".buttonM:focus {background-color:white;}.smallbutton {font-family:\"Comic Sans MS\";font-size:20px;font-weight:bold;line-height:20px;text-align:center;color:#0a0a0a;background-color:#ffe100;padding:20px;width:117px;}");
    client.println(".smallbutton:focus {background-color:white;}.bigfont{font-size: 30px;}");
    client.println("</style>");
   
client.println("</head>");

client.println("<body>");
client.println("<div class=\"row\">");
  client.println("<div class=\"column\">");
    //client.println("<h2>Throttle</h2>");
  client.println("<table>");
      client.println("<tbody>");
        client.println("<tr>");
          client.println("<td><a href=\"/fast\"><button class=\"smallbutton\" >fast</button></a></td>");
        client.println("</tr>");
        client.println("<tr>");
          client.println("<td><a href=\"/medium\"><button class=\"smallbutton\" >medium</button></a></td>");
        client.println("</tr>");
        client.println("<tr>");
          client.println("<td><a href=\"/slow\"><button class=\"smallbutton\" >slow</button></a></td>");
        client.println("</tr>");
      client.println("</tbody>");
    client.println("</table>");
  client.println("</div>");
  
  client.println("<div class=\"column\">");
    //client.println("<h2>Steering</h2>");
  client.println("<table>");
      client.println("<tbody>");
      client.println("<tr>");
      client.println("<td>&nbsp;</td>");
      client.println("<td><a href=\"/fwd\"><button class=\"buttonM\" >FWD</button></a></td>");
      client.println("<td>&nbsp;</td>");
      client.println("</tr>");
      client.println("<tr>");
      client.println("<td><a href=\"/left\"><button class=\"buttonM\" >LEFT</button></a></td>");
      client.println("<td><a href=\"/stop\"><button class=\"buttonM\" >STOP</button></a></td>");
      client.println("<td><a href=\"/right\"><button class=\"buttonM\" >RIGHT</button></a></td>");
      client.println("</tr>");
      client.println("<tr>");
      client.println("<td>&nbsp;</td>");
      client.println("<td><a href=\"/back\"><button class=\"buttonM\" >BACK</button></a></td>");
      client.println("<td>&nbsp;</td>");
      client.println("</tr>");
      client.println("</tbody>");
    client.println("</table>");
  client.println("</div>");
  
client.println("</div>");
client.println("<div class=\"row\">");
  client.println("<div class=\"column\">");
    
  client.println("</div>");
client.println("</div>");
            
            /*
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }*/
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
            } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
