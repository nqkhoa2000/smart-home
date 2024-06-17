#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>

// Create instances
LiquidCrystal_I2C lcd(0x3F, 16, 2);
MFRC522 rfid(10, 9); // MFRC522 mfrc522(SS_PIN, RST_PIN)
Servo sg90;

// Initialize Pins servo
constexpr uint8_t servoPin = 8;


char initial_password[4] = {'1', '2', '3', '4'};  // Variable to store initial password
String cardUID = "53 76 37 25";  // String to store UID of card. Change it with your card's UID
char password[4];   // Variable to store users password
boolean RFIDMode = true; // boolean to change modes
char key_pressed = 0; // Variable to store incoming keys press
uint8_t i = 0;  // Variable used for counter

// defining how many rows and columns our keypad have
const byte rows = 4;
const byte cols = 3;

// Keypad pin map
char Keys[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Initializing pins for keypad
byte rowPins[rows] = {A1, A0, A2, A3};
byte colsPins[cols] = {7, 6, 5};

// Create instance for keypad
Keypad keypad_key = Keypad( makeKeymap(Keys), rowPins, colsPins, rows, cols);

void setup() {
  // Arduino Pin configuration
  Serial.begin(9600);
  sg90.attach(servoPin);  //Declare pin 8 for servo
  sg90.write(0); // Set initial position at 90 degrees

  lcd.init();   // LCD screen
  lcd.backlight();
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();   // Init MFRC522

  lcd.clear(); // Clear LCD screen
}

void loop() {
  // System will first look for mode
  if (RFIDMode == true) {
    lcd.setCursor(0, 0);
    lcd.print("   Door Lock");
    lcd.setCursor(0, 1);
    lcd.print(" Scan Your Card ");

    // Look for new cards
    if ( ! rfid.PICC_IsNewCardPresent()) {
      return;
    }

    // Select one of the cards
    if ( ! rfid.PICC_ReadCardSerial()) {
      return;
    }

    //Reading from the card
    String card = "";
    for (byte j = 0; j < rfid.uid.size; j++)
    {
      card.concat(String(rfid.uid.uidByte[j] < 0x10 ? " 0" : " "));
      card.concat(String(rfid.uid.uidByte[j], HEX));
    }
    card.toUpperCase();

    //Checking the card
    if (card.substring(1) == cardUID)
    {
      // If UID of card is matched.
      lcd.clear();
      lcd.print("Card Matched");
      delay(2000);
      lcd.clear();
      lcd.print("Enter Password:");
      lcd.setCursor(0, 1);
      RFIDMode = false; // Make RFID mode false
    }

    else
    {
      // If UID of card is not matched.
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wrong Card");
      lcd.setCursor(0, 1);
      lcd.print("Access Denied");
      delay(2000);
      lcd.clear();
    }
  }

  // If RFID mode is false, it will look for keys from keypad
  if (RFIDMode == false) {
    key_pressed = keypad_key.getKey(); // Storing keys
    if (key_pressed)
    {
      password[i++] = key_pressed; // Storing in password variable
      lcd.print(key_pressed);
    }
    if (i == 4) // If 4 keys are completed
    {
      delay(200);
      if (!(strncmp(password, initial_password, 4))) // If password is matched
      {
        lcd.clear();
        lcd.print("Pass Accepted");
        delay(2000);
        sg90.write(90); // Door Opened
        delay(3000);
        sg90.write(0); // Door Closed
        lcd.clear();
        i = 0;
        RFIDMode = true; // Make RFID mode true
      }
      else    // If password is not matched
      {
        lcd.clear();
        lcd.print("Wrong Password");
        delay(2000);
        lcd.clear();
        i = 0;
        RFIDMode = true;  // Make RFID mode true
      }
    }
  }
}