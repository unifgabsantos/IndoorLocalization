
const char* ssid = "GabDEV1";            // WIFI
const char* password = "123456789";     // SENHA WIFI

String rota_echo                  = "http://postman-echo.com/post";
String rota_fake                  = "http://192.168.137.1:80/beaconScan";

String macAdress  = "";

void wifiConnect (void)
{
  if(WiFi.status() != WL_CONNECTED)  
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
  
    while(WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay (1000);
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    macAdress = WiFi.macAddress();
    Serial.print    ("MAC - ");
    Serial.println  (macAdress);
  }
}

unsigned int postData (String data)
{
  wifiConnect ();

  int httpResponseCode = 0;

  if(WiFi.status()== WL_CONNECTED)
  {       
    WiFiClient Client;      
    HTTPClient http;       
     
    http.begin(Client, rota_fake);
    http.addHeader("Content-Type", "application/json");
//      http.addHeader("token",  "Fn8fyuvw0AlMXCcDje25huzNjhKVEbNvsozlv7iD6ENRZ4kAjugqGh4f0w96SZZA");
    
    httpResponseCode  = http.POST(data);
    String response       = http.getString();
    
    Serial.println("httpResponseCode: " + String(httpResponseCode));
    Serial.println  ("Response: " + response);
    http.end();

    return (httpResponseCode);
  }
}
