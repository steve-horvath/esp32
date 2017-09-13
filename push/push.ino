#include <PubSubClient.h>
#include <WiFi.h>
#include "DHT.h"
#include <stdlib.h>

// Update these with values suitable for your network.

const char* ssid = "BELL147_RPT";
const char* password = "69443A7C";
const char* mqtt_server = "192.168.1.243";

// Initialize DHT sensor.
const int DHTPin = 16;
#define DHTTYPE DHT22
DHT dht(DHTPin, DHTTYPE);


// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

// Client variables 
// for web server version, can be removed????
char linebuf[80];
int charcount=0;
char output;
long lastMsg = 0;
char msg[50];
int value = 0;


// mqtt publish variables
WiFiClient espClient;
PubSubClient client(espClient);


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    Serial.println(clientId);
    // Attempt to connect
    client.setServer("192.168.1.243",1883);
    if (client.connect(clientId.c_str())) {

      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("ESP32/reconnect", "Module 1 connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


float temperature()
{
        serial.println("Getting value from DHT Sensor");
        DHT_return = 0

        // Make sure that the value returned for temp is valid before moving on 

        while ( t < 1 || t > 50 )
        {
                float t = dht.readTemperature();
                serial.println(" Got temperature %f", t);
        }

        // Make sure that the value returned for Humidity is valid before moving on 


        while ( h < 1 || h > 50 )
        {
                float h = dht.readHumidity();
                serial.println("Got humidity %f", h);
        }



        DHT_return[0] = t
        DHT_return[1] = h

        serial.println("Temp returning is: %f deg. C", DHT_return[0]);
        serial.println("Humidity returning is: %f %", DHT_return[1]);

        return DHT_return;
}


void wifi_connect()
{
        delay(10);
        // We start by connecting to a WiFi network
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
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
}


void publish(float *DHT_return)
{

}

void DHT_sleep()
{

}



// this is the actual program to run, first, setup to establish everything
// then the loop to get the measurements and then publish them

void setup() {
  // initialize the DHT sensor
  dht.begin();

  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while(!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // We start by connecting to a WiFi network

        while (WiFi.status() != WL_CONNECTED)
        {
                wifi_connect();
        }
}



void loop() {

  if (!client.connected()) {
    reconnect();
  }


  client.loop();
  char msg[5];  
  long now = millis();
  if (now - lastMsg > 20000) {
    lastMsg = now;
    ++value;
    Serial.print("Publish message: ");
    Serial.println(t);
    Serial.println(h);
    snprintf(msg, 5, "%f", t);
    client.publish("temp", msg);
    snprintf(msg, 5, "%f", h);
    client.publish("humid", msg);
  }
}
