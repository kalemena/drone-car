/* 
 * Drone receiver remote control test.
 * Receives an integer used as PWM in remote receiver. 
 */

#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

RF24 radio (7, 8);
Servo servo_dir;

byte addresses[][6] = {"1Node"};
int servo_dir_pos;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("RF24/Receiver Test"));

  servo_dir.attach(9);

  radio.begin();
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_MIN);
  
  radio.openReadingPipe(1, addresses[0]);
  radio.startListening();
}

void loop()
{
  servo_dir.write(servo_dir_pos);
  delay(15);
  
  if (radio.available())
  {
    while (radio.available())
    {
      radio.read( &servo_dir_pos, sizeof(servo_dir_pos) ); 
    }
    Serial.print("Data received = ");
    Serial.println(servo_dir_pos);
  }
}
