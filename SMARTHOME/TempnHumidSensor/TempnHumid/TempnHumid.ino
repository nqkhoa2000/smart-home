#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define DHTPIN 16
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

float h, t;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
  dht.begin();
  Serial.print("ESP32 and DHT11");
  Serial.println("initialising...");
  lcd.init();
  lcd.backlight();
  lcd.blink();
  delay(3000);

}

void loop() {
  // put your main code here, to run repeatedly:
  h = dht.readHumidity();
  t = dht.readTemperature();
  Serial.println(t);
  Serial.println(h);
  lcd.clear();
  lcd.blink_off();
  lcd.setCursor(0 , 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(char(223));
  lcd.print("C");

  lcd.setCursor(0 , 1);
  lcd.print("Hum: ");
  lcd.print(h);
  lcd.print("%");
  delay(5000);

}
