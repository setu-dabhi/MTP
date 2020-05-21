#include <DHTesp.h>  

// DHT object
DHTesp dht;
// ESP32 pin used to query DHT22
#define DHT_PIN 23
void setup() {
  dht.setup(DHT_PIN, DHTesp::DHT22);

}

void loop() {
   Serial.begin(115200);
   TempAndHumidity lastValues = dht.getTempAndHumidity();    
   Serial.println(lastValues.humidity);
   Serial.println(lastValues.temperature);
   delay(1000);
    

}
