/* 
 * Drone remote control test.
 * Send an integer used as PWM in remote receiver. 
 */

#include <SPI.h>
#include "RF24.h"

RF24 radio(7, 8);
byte addresses[][6] = {"1Node"};
int servo_dir_pos;

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

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("RF24/Controller Test"));
  
  servo_dir_pos = 0;
  radio.begin();
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_MIN);
  
  radio.openWritingPipe( addresses[0] );
  delay(1000);
}

void loop()
{
  Serial.print(F("Enter value and hit return ..."));
  servo_dir_pos = GetStringNumber();
  Serial.print(F("Value to transmit = ")); Serial.println(servo_dir_pos);
  radio.write( &servo_dir_pos, sizeof(servo_dir_pos) );
  
  delay(500);
}
