
void setup()
{
  Serial.begin(115200);
  pinMode(23, OUTPUT); 
  pinMode(22, OUTPUT);
}


void loop()
{
    Serial.println("LEFT");
    digitalWrite(23,HIGH);
    digitalWrite(22,LOW);
    delay(5000);
    Serial.println("RIGHT");
    digitalWrite(23,LOW);
    digitalWrite(22,HIGH);
    delay(5000);

}
