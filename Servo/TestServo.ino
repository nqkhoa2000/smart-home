#include <Servo.h>

Servo myservo;

int pos = 0;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(8);
}

void loop() {
  // put your main code here, to run repeatedly:
  //for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(90);              // tell servo to go to position in variable 'pos'
    delay(3000);                       // waits 15ms for the servo to reach the position
  //}
  //for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(0);              // tell servo to go to position in variable 'pos'
    delay(3000);                       // waits 15ms for the servo to reach the position
  //}
}
