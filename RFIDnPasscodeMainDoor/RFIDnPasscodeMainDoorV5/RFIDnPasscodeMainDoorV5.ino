#include <Servo.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>


#define RST_PIN         9
#define SS_PIN          10
//#define pinBuzz   0     


Servo myServo;
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x3F,16,2);
 

int UID[4], i;
int ID1[4] = {83, 118, 55, 37}; //Valid Card

const byte ROWS = 4; //4 rows
const byte COLS = 3; //3 columns

// Password variables
char enteredPassword[5]=""; // Assuming a 4-digit password
char correctPassword[] = "1234"; //Default Password
bool continueOpen = false;
//define keypad map
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROWS] = {A1, A0, A2, A3};
byte pin_column[COLS] = {7, 6, 5};

// setlock
int wrongPassword=0; //Wrong Password Entered Attempts
const int maxwrongPassword =3; 
int timelock=0; //Time lock keypad
int count=0;
int flag=0;

//initialize an Keypad 
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROWS, COLS); 

void setup(){
Serial.begin(9600);

//pinMode(pinBuzz,OUTPUT);
SPI.begin();    // rfid 
mfrc522.PCD_Init();
myServo.attach(8);
lcd.init();
lcd.backlight();
lcd.clear();
lcd.setCursor(1, 0);
lcd.print("Scan your Card:");
delay(50);
myServo.write(0);
}

void loop(){ 
if (continueOpen==true){
    step2();
}
else
    step1();
}

void handleRfid(){
    // rfid
    flag=0; 
if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("RFID Card Detected!");
    Serial.print("Card UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      UID[i] = mfrc522.uid.uidByte[i];
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i]);
    }

  if (UID[i] == ID1[i])
  {
    //continueOpen = true;
    if(continueOpen == false)
    {
    lcd.setCursor(0, 1);
    lcd.print("ID Unlock");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("                 ");
    Serial.println("Door Unlock"); 
    continueOpen = true;
    }
    else
    {
    lcd.setCursor(0, 1);
    lcd.print("ID Lock");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("                 ");
    Serial.println("Door Lock"); 
    continueOpen = false;
    }
  }

  else
  {
    continueOpen = false;
    lcd.setCursor(0, 1);
    lcd.print("Access Dennied"); 
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("                "); 
  }
    Serial.println();
    mfrc522.PICC_HaltA();
    delay(1000); // Add delay to avoid multiple readings
  }
}

void keyPad(){
char key = keypad.getKey();
 if (key) {
    if (key == '#') {
      // Check the entered password
      if (strcmp(enteredPassword, correctPassword) == 0) {
        lcd.clear();
        lcd.print("Door Unlock");
        myServo.write(90);
        delay(5000);
        myServo.write(0);
        lcd.clear();
        lcd.print("                 ");
        lcd.setCursor(1, 0);
        //lcd.print("Enter Password:");
        clearEnteredPassword();
        wrongPassword=0;
        continueOpen = false;

      } 
      else {
        lcd.clear();
        lcd.print("Wrong Password");
        delay(2000);
        lcd.clear();
        lcd.print("Enter Password:");
        clearEnteredPassword();
        wrongPassword++;
        if (wrongPassword>=maxwrongPassword){
          //delay(1000);
          lcd.clear();
          lcd.print("Keypad Lock");
        for(timelock=0;timelock<=1000;timelock++){
          delay(10);
        }
        wrongPassword=0;
        lcd.clear();
        //lcd.print("Enter Password:");
        continueOpen = false;
        }
      }
    }
    else if (key == '*') {
      delay(200);
      changePassword();
    } else {
      if (strlen(enteredPassword) < 4) 
      {
        enteredPassword[strlen(enteredPassword)] = key;
        lcd.setCursor(strlen(enteredPassword) - 1, 1);
        lcd.print('*');
      }
    }
  }
}

void clearEnteredPassword() {
  memset(enteredPassword, 0, sizeof(enteredPassword));
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear the password display
  lcd.setCursor(0, 1);
}

void changePassword() {
  lcd.clear();
  lcd.print("Enter New Password:");
  clearEnteredPassword();
  delay(200);
  while (strlen(enteredPassword) < 4) {
    char key = keypad.getKey();
    if (key && key != '*' && key != '#') {
      enteredPassword[strlen(enteredPassword)] = key;
      lcd.setCursor(strlen(enteredPassword) - 1, 1);
      lcd.print('*');
    }
  }
  delay(200);
  memcpy(correctPassword, enteredPassword, sizeof(enteredPassword)); // Copies the entered password to a buffer named correctPassword.
  lcd.clear();
  lcd.print("Password Changed");
  delay(2000);
  lcd.clear();
  lcd.print("Enter Password:");
  clearEnteredPassword();
}


void step1(){
    lcd.setCursor(0, 0);
    lcd.print("Scan your Card:");
    handleRfid();
}

void step2()
{
    lcd.setCursor(0, 0);
    lcd.print("Enter Password:");
    keyPad();
}

