#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>

//define rows and cols keypad
const byte ROWS = 4;
const byte COLS = 3;

//Keypad pin map
char key_Pad[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

//Pins for keypad
byte rowPins[ROWS] = {8, 7, 6, 5}; 
byte colPins[COLS] = {4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(key_Pad), rowPins, colPins, ROWS, COLS); 


void setup(){
  Serial.begin(9600);
}

void loop(){

  char customKey = customKeypad.getKey();
  if (customKey){
    Serial.println(customKey);
  }
}


  