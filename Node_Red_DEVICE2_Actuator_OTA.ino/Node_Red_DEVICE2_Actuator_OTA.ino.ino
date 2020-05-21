#include <WiFi.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//Digital actuation pins

#define Relay_pin_1 11
#define Relay_pin_2 8
#define Relay_pin_3 7
#define Relay_pin_4 6

const char* ssid = "setu";
const char* password = "12345678z";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.43.126";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient1;
PubSubClient client(espClient1);

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
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

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
  
  if(topic=="motor"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(Relay_pin_1, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(Relay_pin_1, LOW);
        Serial.print("Off");
      }
  }

  if(topic=="valve_1"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(Relay_pin_3, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(Relay_pin_2, LOW);
        Serial.print("Off");
      }
  }


  if(topic=="valve_2"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(Relay_pin_3, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(Relay_pin_3, LOW);
        Serial.print("Off");
      }
  }


  if(topic=="battery"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(Relay_pin_4, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(Relay_pin_4, LOW);
        Serial.print("Off");
      }
  }
  Serial.println();
}


// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
  
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client_1")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
  
    if (client.connect("ESP8266Client_1")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("motor");
      client.subscribe("valve_1");
      client.subscribe("valve_2");
      client.subscribe("battery");
      
      
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
  pinMode(Relay_pin_1,OUTPUT);
  pinMode(Relay_pin_2,OUTPUT);
  pinMode(Relay_pin_3,OUTPUT);
  pinMode(Relay_pin_4,OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
    if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client_1");

  /*
  digitalWrite(LEDb, HIGH); 
  delay(50);
  digitalWrite(LEDb, LOW);
  delay(50);
  */ 
}
