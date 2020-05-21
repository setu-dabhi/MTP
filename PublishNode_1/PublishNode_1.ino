#include<Stdio.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DHTesp.h>  
DHTesp dht; //DHT22

#define pin1 32
#define pin2 39
#define pin3 34
#define pin4 35
#define DHT_pin 23


const char* ssid = "setu";
const char* password = "12345678z";
const char* mqtt_server = "192.168.43.126";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

//Initialisation of GPIO Pins
float moisture_pin1 =0;
float moisture_pin2 =0;
float moisture_pin3 =0;
float moisture_pin4 =0;
float humidity_12 =0;
float temperature_12 =0;

char mpin1[10];
char mpin2[10];
char mpin3[10];
char mpin4[10];
char humidity_12_char[10];
char temperature_12_char[10];


void setup_wifi() 
{

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESPClientM5";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);
  pinMode(pin4, INPUT);
  dht.setup(DHT_pin, DHTesp::DHT22);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  moisture_pin1 = analogRead(pin1);
  moisture_pin2 = analogRead(pin2);
  moisture_pin3 = analogRead(pin3);
  moisture_pin4 = analogRead(pin4);
  TempAndHumidity lastValues = dht.getTempAndHumidity();
  humidity_12 = lastValues.humidity;
  temperature_12 = lastValues.temperature;
  
  //Serial.println(moisture_pin1);

  //Float to String conversion
  gcvt(moisture_pin1, 6, mpin1);
  gcvt(moisture_pin2, 6, mpin2);
  gcvt(moisture_pin3, 6, mpin3);
  gcvt(moisture_pin4, 6, mpin4);
  gcvt(humidity_12, 6, humidity_12_char);
  gcvt(temperature_12, 6, temperature_12_char);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    ++value;
    
    Serial.println("Publish message: ");
    Serial.print(moisture_pin1);
    Serial.print(",");
    Serial.print(moisture_pin2);
    Serial.print(",");
    Serial.print(moisture_pin3);
    Serial.print(",");
    Serial.print(moisture_pin4);
    Serial.print(",");
    Serial.print(humidity_12);
    Serial.print(",");
    Serial.println(temperature_12);

    client.publish("MoistureSensor1", mpin1);
    client.publish("MoistureSensor2", mpin2);
    client.publish("MoistureSensor3", mpin3);
    client.publish("MoistureSensor4", mpin4);
    client.publish("MoistureSensor4", mpin4);
    client.publish("Humidity_12", humidity_12_char);
    client.publish("Temperature_12", temperature_12_char);
 
    
  }
}
