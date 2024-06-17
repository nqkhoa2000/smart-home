#define BLYNK_TEMPLATE_ID "TMPL6U8ttRUcb"
#define BLYNK_TEMPLATE_NAME "Googl Assistant Control LED"
#define BLYNK_AUTH_TOKEN "AwibWTio5oG2_MHE81nEZ3-_QW8IZlwl"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFIClient.h>
#include <BlynkSimpleEsp32.h>


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Bôn";
char pass[] = "07082000";


BLYNK_WRITE(V2) {
  digitalWrite(V2, param.asInt());
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(V2, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
}
