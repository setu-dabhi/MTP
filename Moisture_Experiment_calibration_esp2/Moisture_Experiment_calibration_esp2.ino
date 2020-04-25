/*
 *   DHT12
 *  esp32 implicit read on second i2c channel
 *  by Mischianti Renzo <http://www.mischianti.org>
 *
 *  https://www.mischianti.org/2019/01/01/dht12-library-en/
 *
 *
 * DHT12      ----- esp32
 * SDA        ----- 21
 * SCL        ----- 22
 *
 */

#include "Arduino.h"
#include "Wire.h"
//#include <DHT12.h>

// Instantiate Wire for generic use at 400kHz
TwoWire I2Cone = TwoWire(0);
// Instantiate Wire for generic use at 100kHz
TwoWire I2Ctwo = TwoWire(1);

// Set dht12 i2c comunication with second Wire using 21 22 as SDA SCL
//DHT12 dht12(&I2Ctwo);
//DHT12 dht12(&I2Ctwo, 21,22);
//DHT12 dht12(&I2Ctwo, 0x5C);
//DHT12 dht12(&I2Ctwo, 21,22,0x5C);

void setup()
{
  //I2Cone.begin(16,17,400000); // SDA pin 16, SCL pin 17, 400kHz frequency

  Serial.begin(112560);
  // Start sensor
 // dht12.begin();
}
int timeSinceLastRead = 0;

void loop()
{
  // Report every 2 seconds.
  if(timeSinceLastRead > 2000) {
        Serial.print(analogRead(36));
      Serial.print(",");
      Serial.print(analogRead(39));
      Serial.print(",");
      Serial.print(analogRead(34));
      Serial.print(",");
      Serial.println(analogRead(35));  

   
    timeSinceLastRead = 0;

  }
  delay(100);
  timeSinceLastRead += 100;
}
