/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
String inString = "";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Specify position: ");
    
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

int GetStringNumber()
{
   int value = 0;
       
   while(1)
   {
       char byteBuffer = Serial.read();
       if(byteBuffer == '\n')
         break;

       if(byteBuffer > -1) 
       {
         if(byteBuffer >= '0' && byteBuffer <= '9')
           value = (value * 10) + (byteBuffer - '0');
         else
           break;       
       }
   }

  return value;
}

void loop() {
// 60 - 120 for direction
  

for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  // in steps of 1 degree
  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(15);                       // waits 15ms for the servo to reach the position
}
for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(15);                       // waits 15ms for the servo to reach the position
}

/*
int pos = GetStringNumber();
Serial.print("Value:"); Serial.println(pos);
myservo.write(pos);
delay(15);
*/
}

