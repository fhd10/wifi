#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "DHT.h"

const char *ssid = "HUAWEI P40";
const char *password = "123456781";
AsyncWebServer server(70);

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int Prox1 = D5;
const int Prox2 = D7;
const int Prox3 = D8;

boolean car = false;
boolean temp = false;
boolean slot = false;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ; /* code */
  }
  dht.begin(); // initialize the sensor
  pinMode(Prox1, INPUT);
  pinMode(Prox2, INPUT);
  pinMode(Prox3, INPUT);

  WiFi.begin(ssid, password);
  Serial.print(" \nConnected to the WiFi network: ");
  Serial.print(WiFi.SSID());
  Serial.print(" IP address: ");
  Serial.print(WiFi.localIP());

  server.on("/car", HTTP_POST, [](AsyncWebServerRequest *request) {
    car = !car;
    request->send(200);
  });

  server.on("/temp", HTTP_POST, [](AsyncWebServerRequest *request) {
    temp = !temp;
    request->send(200);
  });

  server.on("/slot", HTTP_POST, [](AsyncWebServerRequest *request) {
    slot = !slot;
    request->send(200);
  });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", " * ");
  server.begin();
}

void loop()
{

  while (temp == true)
  {
    // wait a few seconds between measurements.
    delay(2000);

    // read humidity
    float humi = dht.readHumidity();
    // read temperature as Celsius
    float tempC = dht.readTemperature();

    // check if any reads failed
    if (isnan(humi) || isnan(tempC))
    {
      Serial.println("Failed to read from DHT sensor!");
    }
    else
    {
      Serial.print("Humidity: ");
      Serial.print(humi);
      Serial.println("%");

      Serial.println("  |  ");

      Serial.print("Temperature: ");
      Serial.print(tempC);
      Serial.println("°C ~ ");
    }
  }

  while (slot == true)
  {
    {
      if (digitalRead(Prox1) == HIGH) //Check the sensor output
      {
        Serial.println("(1)=empty");
      }
      else
      {
        Serial.println("(1)=full");
      }
    }

    delay(1000); // wait for a second

    {
      if (digitalRead(Prox2) == HIGH) //Check the sensor output
      {
        Serial.print("(2)=empty");
      }
      else
      {
        Serial.print("(2)=full");
      }
    }

    delay(1000); // wait for a second

    {
      if (digitalRead(Prox3) == HIGH) //Check the sensor output
      {
        Serial.print("(3)=empty");
      }
      else
      {
        Serial.println("(3)=full");
      }
    }

    delay(1000); // wait for a second
  }
  while (car == true)
  {
    if (Serial.available())
    {
      Serial.write(Serial.read());
    }
  }
}
