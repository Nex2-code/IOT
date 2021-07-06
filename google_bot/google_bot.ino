
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "ge2"
#define WIFI_PASS "123456781"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "globalapi01"
#define MQTT_PASS "9b78813574bc40a4bf58bb48b54217cd"
#define m D1
#define m1 D2
#define m2 D3
#define m3 D4

//Set up MQTT and WiFi clients
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

//Set up the feed you're subscribing to
Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/bot");


void setup()
{
  Serial.begin(115200);

  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("OK!");

  mqtt.subscribe(&onoff);

  pinMode(m, OUTPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  digitalWrite(m, LOW);
  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
  digitalWrite(m3, LOW);
}

void loop()
{
  MQTT_connect();
  
  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &onoff)
    {
      //Print the new value to the serial monitor
      Serial.print("onoff: ");
      Serial.println((char*) onoff.lastread);
      
      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) onoff.lastread, "ON"))
      {
        //Active low logic
        digitalWrite(m, HIGH);
        digitalWrite(m1,LOW);               //Forward         
        digitalWrite(m2,HIGH);
        digitalWrite(m3,LOW);
      }
      else if (!strcmp((char*) onoff.lastread, "ON4"))
      {
        digitalWrite(m, LOW);
        digitalWrite(m1,LOW);         // Stop
        digitalWrite(m2,LOW);
        digitalWrite(m3,LOW);
      }
       else if (!strcmp((char*) onoff.lastread, "ON3"))
      {
        digitalWrite(m, HIGH);
        digitalWrite(m1,LOW);
        digitalWrite(m2,LOW);
        digitalWrite(m3,LOW);
      }
         else if (!strcmp((char*) onoff.lastread, "ON2"))
      {
        digitalWrite(m, LOW);
        digitalWrite(m1,LOW);
        digitalWrite(m2,HIGH);
        digitalWrite(m3,LOW);
      }
       else if (!strcmp((char*) onoff.lastread, "ON1"))
      {
        digitalWrite(m, LOW);
        digitalWrite(m1,HIGH);
        digitalWrite(m2,LOW);
        digitalWrite(m3,HIGH);
      }  
    }
  }
  // ping the server to keep the mqtt connection alive
  if (!mqtt.ping())
  {
    mqtt.disconnect();
  }
}

void MQTT_connect() 
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
