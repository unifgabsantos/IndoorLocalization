#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "esp_bt_device.h"
#include <ArduinoJson.h>

#define TARGET_RSSI -70

int scanTime = 2;
BLEScan* pBLEScan;

String macAdress = "AA:BB:CC";

String  detectedNames[100];
String  detectedMACs[100];
int     detectedRSSIs[100];
unsigned int counterDetected = 0;

int arrayRSSI[20] = {0,0,0,0,0,0,0,0,0,0};
byte pointer = 0;
byte f_firstTime = 1;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{
  void onResult(BLEAdvertisedDevice advertisedDevice) 
  {
    String deviceInfo = advertisedDevice.toString().c_str();
    
    int delimiter = deviceInfo.indexOf(",");      
    String deviceName = deviceInfo.substring(6, delimiter);
    
    int rssi          = advertisedDevice.getRSSI();
    String macAddr    = advertisedDevice.getAddress().toString().c_str();
    macAddr.toUpperCase();

    detectedNames[counterDetected] = deviceName;
    detectedMACs[counterDetected] = macAddr;
    detectedRSSIs[counterDetected] = rssi;

    if (counterDetected < 98) counterDetected++;

    // FC:58:FA:C4:C5:F3 Relogio
    // 7C:9E:BD:F0:7B:26 ESP Lopes
    // ITAG FC:58:FA:C4:C5:F3
    
    if(macAddr == "D2:62:76:02:B0:D0")
    {
      Serial.println ("Achei = " + macAddr + " - " + String (rssi));
      
      arrayRSSI[pointer] = rssi;
      ++pointer;
      if (pointer >= 20)  pointer = 0;

      if (f_firstTime == 1)
      { 
        for (byte i = 0; i < 20; ++i)
          arrayRSSI[i] = rssi;

        f_firstTime = 0;
      }
    }
    

//    Serial.println (deviceName + "," + macAddr + "," + String (rssi));
  }
};

unsigned int sendScans (unsigned int numDetected)
{
  DynamicJsonDocument doc(16384);
  JsonObject docObj = doc.to<JsonObject>();
  docObj["scannerMAC"] = macAdress;

  JsonArray data;

  data = doc.createNestedArray("scans");
  
  for (byte j = 0; j < numDetected; ++j)
  {
    DynamicJsonDocument doc2(4096);

    if (detectedMACs[j] == "7C:9E:BD:F0:7B:26")
    {
      Serial.println ("");
      
      int sum = 0;
      int media = 0;
      for (byte i = 0; i < 20; ++i)
      {
        sum += arrayRSSI[i];
        Serial.println (arrayRSSI[i]);
      }

      media = (sum / 20);

      Serial.print ("/////");
      Serial.print (abs(media - detectedRSSIs[j]));

      if ((abs(media - detectedRSSIs[j])) >= 10)
      {
        for (byte i = 0; i < 20; ++i)
          arrayRSSI[i] = detectedRSSIs[j];
      }
      
      doc2["detectedName"]  = detectedNames[j];
      doc2["detectedMAC"]   = detectedMACs[j];
      doc2["detectedRSSI"]  = media;

      Serial.print ("\nMedia: ");
      Serial.println (media);

      
    }
    else
    {
      doc2["detectedName"]  = detectedNames[j];
      doc2["detectedMAC"]   = detectedMACs[j];
      doc2["detectedRSSI"]  = detectedRSSIs[j];
    }

    JsonObject obj = doc2.as<JsonObject>();
    data.add(obj);
  }

  String dataToSend = "";
  
  serializeJsonPretty(doc, Serial);
  serializeJson(doc, dataToSend);
  Serial.println ();

  
  Serial2.print ("$");
  for (int i = 0; i < dataToSend.length(); ++i)
  {
    Serial2.print (dataToSend[i]);
    delayMicroseconds (1000);
  }
  Serial2.print ("#");
}

String getMACinString() 
{
  const uint8_t* macAddress = esp_bt_dev_get_address();
  char charMAC[18];

  sprintf(charMAC, "%02X", (int)macAddress[0]);
  charMAC[2] = ':';
  sprintf(charMAC+3, "%02X", (int)macAddress[1]);
  charMAC[5] = ':';

  sprintf(charMAC+6, "%02X", (int)macAddress[2]);
  charMAC[8] = ':';
  sprintf(charMAC+9, "%02X", (int)macAddress[3]);
  charMAC[11] = ':';

  sprintf(charMAC+12, "%02X", (int)macAddress[4]);
  charMAC[14] = ':';
  sprintf(charMAC+15, "%02X", (int)macAddress[5]);

  return (String)charMAC;
}

void setup() 
{
  Serial.begin(115200);
  Serial2.begin (115200);

  pinMode (2, OUTPUT);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); 
  pBLEScan->setInterval(100); //100
  pBLEScan->setWindow(99);   //99

  macAdress = getMACinString();
  Serial.println("My MAC address = " + macAdress);
}

void loop() 
{
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  unsigned int numDetected = foundDevices.getCount();  
  Serial.print("Devices found: ");
  Serial.println(numDetected);  
  Serial.println("Scan done!\n");  
  pBLEScan->clearResults();   
  counterDetected = 0;

  sendScans (numDetected);

//  delay (3000);
}
