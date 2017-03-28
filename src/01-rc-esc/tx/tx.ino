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

uint8_t data[2];
uint8_t lastData[2];

int dir_pin=6;
int speed_pin=7;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("RF24/Controller Test"));

  data[0] = 90; // center
  data[1] = 95; // stop
  
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
  data[0] = map(val, 0, 1023, 60, 119);

  val = analogRead(speed_pin);
  data[1] = map(val, 0, 1023, 30, 160);
      
  if(lastData[0] < (data[0]-1) || lastData[1] < (data[1]-1)
    || lastData[0] > (data[0]+1) || lastData[1] > (data[1]+1)
    || ((micros()/1000) - send_at > 500 ) ) {
      
    radio.write( &data, sizeof(data) );
    lastData[0] = data[0];
    lastData[1] = data[1];

    char buf[256];
    sprintf(buf, "Dir = %3d - Speed = %3d\n", data[0], data[1]);
    Serial.print(buf);

    send_at = micros() / 1000;
  }

  delay(8);    
}


