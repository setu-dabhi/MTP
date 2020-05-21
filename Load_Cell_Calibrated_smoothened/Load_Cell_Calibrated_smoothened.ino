/*Load Cell Caliberation using Indian Rupee Coins
 * STANDARD WEIGHT : https://m.rbi.org.in/Scripts/restrospectcoins.aspx
 * 1 Rupee = 3.79gm
 * 2 Rupee = 4.85gm
 * 5 Rupee (Brass) = 6gm
 * 10 Rupee = 7.71gm
 * Caliberation equation is Static for the experiment obtained by linear regression in MInitab
 */

#include <Q2HX711.h>

const byte hx711_data_pin = 7;
const byte hx711_clock_pin = 18;



float a = 0;
float b = 0;
float y = 0;
float weight =0;

//long float x = 0;

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);

const int numReadings = 5; //Weighted moving Average of 10 Numbers

int readings[numReadings];      // the readings from the analog input 10 numbers

int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average
float average15 = 0;
float average20 = 0;


void setup() {
  Serial.begin(115200);
  
  //For 10
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }  
}

void loop() {

  // subtract the last reading:
  total = total - readings[readIndex];
    
  // read from the sensor:
  a = hx711.read()/100.0;
  readings[readIndex] = a; 
  
  // add the reading to the total:
  total = total + readings[readIndex];
    
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  average15 = total / 15;
  average20 = total / 20;

  // send it to the computer as ASCII digits
  weight =0;
  //weight = (38854.2 - 0.462211*average);
  //weight =(38854.2 - (0.46221*average));
  //Serial.print(a);
 // Serial.print(",");
 // Serial.println(weight);
  
  
  Serial.print("average 10 :"); //Average of 10
  Serial.print(average); //Average of 10
  Serial.print(",");
  Serial.print("average 15 :"); //Average of 10
  Serial.print(average15); //Average of 15
  Serial.print(",");
  Serial.print("average 20 :"); //Average of 10
  Serial.print(average20); //Average of 20
  Serial.print(",");
  Serial.print("No Average :"); 
  Serial.println(a); // No Averaging
  
  
  delay(100);
}
