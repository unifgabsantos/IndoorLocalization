#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

String message = "";

void setup() 
{
  Serial.begin(115200); 
  Serial2.begin (115200);

  pinMode (4, OUTPUT);
  digitalWrite (4, 0);

  wifiConnect ();

  digitalWrite (4, 1);
  delay (200);
  digitalWrite (4, 0);
  delay (200);
  digitalWrite (4, 1);
  delay (200);
  digitalWrite (4, 0);
  delay (200);
}

void loop() 
{  
  if (Serial2.available() > 0)
  {
    char i;
    while (Serial2.available() > 0)
    {
      i = char (Serial2.read());
      
      message += i;
    }
    if (i == '#')
    {
      message.replace ("$", "");
      message.replace ("#", "");
      Serial.println (message);  

      if (postData (message) == 200)
      {
        digitalWrite (4, 1);
        delay (80);
        digitalWrite (4, 0);
        delay (80);
        digitalWrite (4, 1);
        delay (80);
        digitalWrite (4, 0);
        delay (80);
      }
      
      message = "";
    }
  }
}
