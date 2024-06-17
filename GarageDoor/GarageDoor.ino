#define BLYNK_TEMPLATE_ID "TMPL63uFcOZJI"
#define BLYNK_TEMPLATE_NAME "Garage Door"
#define BLYNK_AUTH_TOKEN "zu7SPoCnZUnWDjBORREFjRg26QjdkPKe"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

char auth[] = BLYNK_AUTH_TOKEN; // Blynk authentication token
char ssid[] = "Bôn"; // Your WiFi SSID
char pass[] = "07082000"; // Your WiFi password

Servo servo1;
Servo servo2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  servo1.attach(2); // Attach servo 1 to GPIO 2
  servo2.attach(4); // Attach servo 2 to GPIO 4
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
}

BLYNK_WRITE(V0) // Button Switch Widget for Servo 1 and 2 on V0
{
  int pos1 = param.asInt(); // Get value from slider
  servo1.write(pos1); // Set servo 1 position (postion min is 0)
  servo2.write(90-pos1); //Set servo 2 position opposite 
  Blynk.virtualWrite(V1, pos1); 
}

/*BLYNK_WRITE_2(V0) // Slider Widget for Servo 2 on V1
{
  int pos1 = param.asInt(); // Get value from slider
  servo.write(pos1); // Set servo 2 position
  Blynk.virtualWrite(V2, pos1);
}*/

