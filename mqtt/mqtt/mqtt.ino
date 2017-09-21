#include <PubSubClient.h>
#include <WiFi.h>
#include "DHT.h"
#include <stdlib.h>

// Update these with values suitable for your network.

const char* ssid = "BELL147";
const char* password = "69443A7C";
const char* mqtt_server = "192.168.1.243";
const int DHTPin = 16;
#define DHTTYPE DHT22

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

// Client variables 
char linebuf[80];
int charcount=0;
char output;

WiFiClient espClient;
PubSubClient mqtt_client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// Server properties
char *mqttServer = "192.168.1.243";
int mqttPort = 1883;
String clientName = "ESP32-1-";
String clientId = "";


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print(millis());
  Serial.print("  Wi-Fi Disconnected.  Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(millis());
    Serial.println("  connecting.");
  }

  Serial.println("");
  Serial.print(millis());
  Serial.print("  WiFi connected - ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void mqtt_connect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) 
  {
    Serial.print(millis());
    Serial.println("  Attempting MQTT connection...");
    // Create a random client ID
    clientId=clientName;
    randomSeed(micros());
    clientId += String(random(0xffff), HEX);
    Serial.print(millis());
    Serial.print("  Client ID - ");
    Serial.println(clientId);
    // Attempt to connect
    mqtt_client.setServer(mqttServer,mqttPort);
    if (mqtt_client.connect(clientId.c_str())) 
    {
      Serial.print(millis());
      Serial.println("  MQTT connection");// to %d:%i", mqttServer,mqttPort);
      // Once connected, publish an announcement...
      mqtt_client.publish("ESP32/reconnect", "Module 1 connected");
    } 
    else 
    {
      Serial.print(millis());
      Serial.print("  failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}




void setup() {
  // initialize the DHT sensor
  dht.begin();
  
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while(!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print(millis());
  Serial.print("  Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  // attempt to connect to Wifi network:
  while(WiFi.status() != WL_CONNECTED) {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    delay(500);
    Serial.print(millis());
    Serial.println("  Connecting . . . ");
  }
  Serial.println("");
  Serial.print(millis());
  Serial.print("  WiFi connected - ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void mqtt_publish(int temp, int humid)
{
//    Serial.print("Publish message: ");
    Serial.print("Temp is reading as ");
    Serial.println(temp);
    Serial.print("Humid is reading as ");
    Serial.println(humid);
    snprintf(msg, 5, "%d", temp);
    Serial.print(millis());
    Serial.print("  Temp Messge:  ");
    Serial.println(msg);
    mqtt_client.publish("temp", msg);
    snprintf(msg, 5, "%d", humid);
    Serial.print(millis());
    Serial.print("  Humid Messge:  ");
    Serial.println(msg);
    mqtt_client.publish("humid", msg);
}


void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
  
  if (!mqtt_client.connected()) {
    mqtt_connect();
  }

  float t=dht.readTemperature();;
  float h=dht.readHumidity();;

  mqtt_publish(t, h);
  Serial.print(millis());
  Serial.println("  Starting Delay");
  delay(30000);
  Serial.print(millis());
  Serial.println("  End Delay");
}
