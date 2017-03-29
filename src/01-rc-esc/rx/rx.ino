/* 
 * Drone receiver remote control test.
 * Receives an integer used as PWM in remote receiver. 
 */

#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

#define DEBUG true
#define Serial if(DEBUG)Serial

RF24 radio (7, 8);
Servo servo_dir;
Servo servo_speed;

byte addresses[][6] = { "1Node" };

struct radioControl {
  uint8_t dir;
  uint8_t speed;
};
radioControl data, lastData;

int cycle = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("RF24/Receiver Test"));

  servo_dir.attach(9);
  servo_speed.attach(5);

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
    sprintf(buf, "Dir = %3d->%3d - Speed = %3d->%3d - Cycle = %3d\n", lastData.dir, data.dir, lastData.speed, data.speed, cycle++);
    Serial.print(buf);

    // direction
    if(lastData.dir != data.dir) {
      servo_dir.write(data.dir);
      lastData.dir = data.dir;    
      delay(25 * abs(lastData.dir-data.dir));
    }

    // speed
    if(lastData.speed != data.speed) {

      // stop / backward
      if(data.speed <= 90 && lastData.speed > 90) {
        stopped_at = micros() / 1000;
        Serial.print("stopped at "); Serial.println(stopped_at);
      }

      if(data.speed < 86) {
        // backward

        // let 1000ms before going back
        if (((micros() /1000) - stopped_at) > 1000 ) 
        {
          servo_speed.write(data.speed);
        } 
        else
        {
          servo_speed.write(88);
        }
      }
      else
      {
        // forward
        servo_speed.write(data.speed);
      }
      
      lastData.speed = data.speed;
      delay(10);
    }
  }
}
