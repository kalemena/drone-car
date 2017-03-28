/* 
 * Drone receiver remote control test.
 * Receives an integer used as PWM in remote receiver. 
 */

#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

#define DEBUG false
#define Serial if(DEBUG)Serial

RF24 radio (7, 8);
Servo servo_dir;
Servo servo_speed;

byte addresses[][6] = { "1Node" };

uint8_t data[2];
int cycle = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("RF24/Receiver Test"));

  servo_dir.attach(5);
  servo_speed.attach(9);

  radio.begin();
  radio.setChannel(108);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  //radio.setCRCLength(RF24_CRC_8);
  //radio.setPayloadSize(sizeof(data));
  
  radio.openReadingPipe(1, addresses[0]);
  radio.startListening();

  servo_dir.write(90-10);
  delay(500);
  servo_dir.write(90+10);
  delay(500);
  servo_dir.write(90);
}

uint8_t lastData[2];
unsigned long stopped_at = 0;
void loop()
{  
  unsigned long started_waiting_at = micros();
  boolean timeout = false;

  while ( !radio.available() ) {
    if (micros() - started_waiting_at > 200000 ){ 
      timeout = true;
      break;
    }
  }

  if ( !timeout ) {    
    radio.read( &data, sizeof(data) );

    char buf[256];
    sprintf(buf, "Dir = %3d->%3d - Speed = %3d->%3d - Cycle = %3d\n", lastData[0], data[0], lastData[1], data[1], cycle++);
    Serial.print(buf);

    // direction
    if(lastData[0] != data[0]) {
      servo_dir.write(data[0]);
      lastData[0] = data[0];    
      delay(25 * abs(lastData[0]-data[0]));
    }

    // speed
    if(lastData[1] != data[1]) {

      // stop / backward
      if(data[1] <= 90 && lastData[1] > 90) {
        stopped_at = micros() / 1000;
        Serial.print("stopped at "); Serial.println(stopped_at);
      }

      if(data[1] < 86) {
        // backward

        // let 1000ms before going back
        if (((micros() /1000) - stopped_at) > 1000 ) 
        {
          servo_speed.write(data[1]);
        } 
        else
        {
          servo_speed.write(88);
        }
      }
      else
      {
        // forward
        servo_speed.write(data[1]);
      }
      
      lastData[1] = data[1];
      delay(10);
    }
  }
}
