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

int pos1 = 0;
int pos2 = 0;

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

BLYNK_WRITE(V0) // Button Widget in SWITCH mode for controlling servos
{
  int buttonState = param.asInt(); // Get the state of the button (0 or 1)

  if (buttonState == 1) { // If the button is pressed
    // Move servo 1 to position pos1
    servo1.write(pos1);
    // Move servo 2 to position 180 - pos2 (opposite position)
    servo2.write(90 - pos2);
  } else { // If the button is released
    // Reset servo positions to initial positions
    servo1.write(0);
    servo2.write(90);
  }
}

BLYNK_WRITE(V2) // Slider Widget for controlling pos1
{
  pos1 = param.asInt(); // Get value from slider
}

BLYNK_WRITE(V1) // Slider Widget for controlling pos2
{
  pos2 = param.asInt(); // Get value from slider
}