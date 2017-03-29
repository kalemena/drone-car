/* 
 * Drone remote control test.
 * Send an integer used as PWM in remote receiver. 
 */

#include <SPI.h>
#include "RF24.h"

#define DEBUG true
#define Serial if(DEBUG)Serial

RF24 radio(7, 8); 
byte addresses[][6] = { "1Node" };

struct radioControl {
  uint8_t dir;
  uint8_t speed;
};

radioControl data, lastData;

int dir_pin=1;
int speed_pin=0;
boolean switch_pin=2;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("RF24/Controller Test"));

  data.dir = 90; // center
  data.speed = 95; // stop

  pinMode(switch_pin, INPUT);
  digitalWrite(switch_pin, HIGH);
  
  radio.begin();
  radio.setChannel(108);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);  
  //radio.setCRCLength(RF24_CRC_8);
  //radio.setPayloadSize(sizeof(data));
   
  radio.openWritingPipe( addresses[0] );
  // radio.printDetails();
  
  delay(1000);
}

unsigned long send_at = 0;
void loop()
{
  int val = analogRead(dir_pin);
  data.dir = map(val, 0, 1023, 119, 60);

  val = analogRead(speed_pin);
  data.speed = map(val, 0, 1023, 60, 120);

  boolean switchVal = digitalRead(switch_pin);
      
  if(lastData.speed < (data.speed-1) || lastData.dir < (data.dir-1)
    || lastData.speed > (data.speed+1) || lastData.dir > (data.dir+1)
    || ((micros()/1000) - send_at > 500 ) ) {
      
    radio.write( &data, sizeof(data) );
    lastData.dir = data.dir;
    lastData.speed = data.speed;

    char buf[256];
    sprintf(buf, "Dir = %3d - Speed = %3d - Switch = %d\n", data.dir, data.speed, switchVal);
    Serial.print(buf);

    send_at = micros() / 1000;
  }

  delay(8);    
}


