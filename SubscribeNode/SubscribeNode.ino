#include <WiFi.h>
#include <PubSubClient.h>


const char* ssid = "setu";
const char* password = "12345678z";

// MQTT broker
const char* mqtt_server = "192.168.43.126";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

//**************************************RELAY-SWITCH-INITIALISATION*****************************

//Switches : 22,23,4,17

const int LED = 2;
const int pin1 = 22; 
const int pin2 = 23; 
const int pin3 = 17; 
//const int pin4 = 4; 
//const int pin5 = 17; 



void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}
  


void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
 
  //*************************SWITCH-ONE*******************************************
  if(topic=="switch1_one")
   {
      Serial.print("Changing Room Switch 1 to ");
      if(messageTemp == "on")
      {
        digitalWrite(pin1, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off")
      {
        digitalWrite(pin1, LOW);
        Serial.print("Off");
      }
  }
  
//***************************SWITCH-TWO*******************************************
   if(topic=="switch2_two")
   {
      Serial.print("Changing Room Switch 2 to ");
      if(messageTemp == "on")
      {
        digitalWrite(pin2, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(pin2, LOW);
        Serial.print("Off");
      }
  }
//****************************SWITCH-THREE**********************************************
   if(topic=="switch3_three")
   {
      Serial.print("Changing Room Switch 3 to ");
      if(messageTemp == "on")
      {
        digitalWrite(pin3, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(pin3, LOW);
        Serial.print("Off");
      }
  }

//******************************SWITCH-LED*********************************************
   if(topic=="switch_led")
   {
      Serial.print("Changing Room LED to ");
      if(messageTemp == "on")
      {
        digitalWrite(LED, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(LED, LOW);
        Serial.print("Off");
      }
  }
  /*
//******************************SWITCH-FOUR************************************
   if(topic=="switch_four"){
      Serial.print("Changing Room pin1 to ");
      if(messageTemp == "on"){
        digitalWrite(pin5, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(pin5, LOW);
        Serial.print("Off");
      }
  }
*/
  
  Serial.println();
}

//Connecting to MQTT Broker and Subsctibing to the Topics
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) //Client's Name
    {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      
      client.subscribe("switch1_one");
      client.subscribe("switch2_two");
      client.subscribe("switch3_three");
      client.subscribe("switch_led");
      //client.subscribe("led_switch");
      
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() 
{
  Serial.begin(115200);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  //pinMode(pin4, OUTPUT);
  pinMode(LED, OUTPUT);
  setup_wifi();
  //Sets the MQTT Server
  client.setServer(mqtt_server, 1883); 
  //The callback is set and is used to control the relay from the messages recieved by MQTT Broker
  client.setCallback(callback); 

}

void loop() 
  {
    if (!client.connected()) 
    {
    reconnect();
    }
    
    if(!client.loop())
    {
    client.connect("ESP8266Client");
    }
}
