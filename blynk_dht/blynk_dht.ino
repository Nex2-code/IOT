#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>       
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>
char auth[] = "Zl6hrr8SiTDJEtOacTyXepoMpDr3z0_-"; //Enter the Auth code which was send by Blink
char ssid[] = "ge2";  //Enter your WIFI Name
char pass[] = "12356712";  //Enter your WIFI Password
#define DHTPIN 2          // Digital pin 4
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, h);  //V5 is for Humidity
  Blynk.virtualWrite(V1, t);  //V6 is for Temperature
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
