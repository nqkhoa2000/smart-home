#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>


//#define RELAY_PIN 12
#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 8
#define MAX_ATTEMPTS 3
#define LOCK_TIME 180000

//Create Instance
LiquidCrystal_I2C lcd(0x3F, 16, 2);
MFRC522 rfid(SS_PIN,RST_PIN);
Servo servo;
//int RELAY_PIN = 12;


//define rows and cols keypad
const byte ROWS = 4;
const byte COLS = 3;

//Keypad pin map
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

//Pins for keypad
byte rowPins[ROWS] = {A1, A0, A2, A3}; 
byte colPins[COLS] = {7, 6, 5}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

char storedPassword[] = "1234"; // Change this to your initial password
char enteredPassword[5] = "";
String cardUID = "53 76 37 25";
int attempts = 0;
unsigned long lockoutStartTime = 0;
bool setPasswordMode = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Enter passcode");
  lcd.setCursor(0,1);
  lcd.print("Scan your card");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - lockoutStartTime < LOCK_TIME) {
    // System is locked out, exit loop
    return;
  }

  // Scan RFID card
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    if (validateRFID()) {
      unlockDoor();
      delay(1000); // Adjust delay time as needed
      return;
    }
  }

  // Passcode keypad input
  char key = keypad.getKey();
  if (key != NO_KEY) {
    if (key == '#') {
      if (setPasswordMode) {
        setPassword();
      } else {
        validatePassword();
      }
    } else if (key == '*') {
      setPasswordMode = true;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set new password");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter new code");
      strcpy(enteredPassword, "");
    } else {
      if (strlen(enteredPassword) < 4) {
        enteredPassword[strlen(enteredPassword)] = key;
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(enteredPassword);
      }    
    }
  }
}

//Scan RFID Card
bool validateRFID() {
  byte authorizedUID[] = {53, 76, 37, 25};

  //Compare the scanned UID with the authorized UID
  if (rfid.uid.size != sizeof(authorizedUID)) {
    lcd.clear();
    lcd.print("Dennied");
    return false;
  }

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] != authorizedUID[i]) {
      lcd.clear();
      lcd.print("Dennied");
      return false;
    }
  }

  lcd.clear();
  lcd.print("Access Granted");
  return true;
}

void validatePassword() {
  if (strcmp(enteredPassword, storedPassword) == 0) {
    unlockDoor();
  } else {
    attempts++;
    if (attempts >= MAX_ATTEMPTS) {
      lockoutStartTime = millis();
      delay(1000);
      attempts = 0;
      strcpy(enteredPassword, "");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("System Locked");
      lcd.setCursor(0,1);
      lcd.print("Try again in 3 mins");
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Wrong password");
      delay(1000); // Display the message for 1 second
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter passcode");
      lcd.setCursor(0,1);
      lcd.print("Scan your card");
    }
  }
}

void setPassword() {
  strcpy(storedPassword, enteredPassword);
  setPasswordMode = false;
  strcpy(enteredPassword, "");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Password set");
  delay(1000); // Display the message for 1 second
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter passcode");
  lcd.setCursor(0,1);
  lcd.print("Scan your card");
}

void unlockDoor() {
  servo.write(90); // Unlock position
  //digitalWrite(RELAY_PIN, HIGH);
  delay(1000); // Adjust delay time as needed
  //digitalWrite(RELAY_PIN, LOW);
  servo.write(0); // Lock position
  strcpy(enteredPassword, "");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Door Unlocked");
  delay(1000); // Display the message for 1 second
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter passcode");
  lcd.setCursor(0,1);
  lcd.print("Scan your card");
}

