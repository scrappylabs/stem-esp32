// ESP32 Touch Test
// Just test touch pin - Touch7 is T7 which is on GPIO 27.

void setup() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");
}

void loop() {
  Serial.println(touchRead(27));  // get value of Touch 7 pin = GPIO 27
  delay(1000);
}
