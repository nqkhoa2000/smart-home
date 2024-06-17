#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6U8ttRUcb"
#define BLYNK_TEMPLATE_NAME "Googl Assistant Control LED"
#define BLYNK_AUTH_TOKEN "AwibWTio5oG2_MHE81nEZ3-_QW8IZlwl"


#include <WiFi.h>
#include <BlynkSimpleEsp32.h>


#define LEDRed 4
#define LEDGreen 16
int button1, button2;

char auth[] = BLYNK_AUTH_TOKEN; // Blynk authentication token
char ssid[] = "Bôn"; // Your WiFi SSID
char pass[] = "07082000"; // Your WiFi password

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(LEDRed, OUTPUT);
  pinMode(LEDGreen, OUTPUT);
}

BLYNK_WRITE(V0)
{
  button1 = param.asInt();
  if (button1 == 1){
    digitalWrite(LEDRed, HIGH);
  }
  else {
    digitalWrite(LEDRed, LOW);
  }
}

BLYNK_WRITE(V1)
{
  button2 = param.asInt();
  if (button2 == 1){
    digitalWrite(LEDGreen, HIGH);
  }
  else {
    digitalWrite(LEDGreen, LOW);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
}
