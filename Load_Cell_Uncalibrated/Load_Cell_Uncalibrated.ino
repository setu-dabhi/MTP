#include <Q2HX711.h>

const byte hx711_data_pin = 7;
const byte hx711_clock_pin = 18;

//const byte hx711_data_pin2 = 17;
//const byte hx711_clock_pin2 = 18;


float a = 0;
float b = 0;
float y = 0;
//long float x = 0;

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);
//Q2HX711 hx7112(hx711_data_pin2, hx711_clock_pin);

void setup() {
  Serial.begin(115200);
}

void loop() {
  //Serial.println(hx711.read());
  a = hx711.read()/100.0;
  //b = hx7112.read()/100.0;
  //y = 7037 - 0.08341*a;
  //Serial.println(a); //This is 5kg
  //Serial.print(",");
  Serial.println(a);
  delay(1000);
}
