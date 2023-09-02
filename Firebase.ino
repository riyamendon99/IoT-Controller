#include <ArduinoJson.h>

#include <Firebase_Arduino_WiFi101.h>
#include <Firebase_Arduino_WiFi101_HTTPClient.h>

#include <dht.h>

#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
//#include "FirebaseArduino.h"
#include "DHT.h" 

#define FIREBASE_HOST "https://iotcontroller-9e3c2.firebaseio.com/" 
#define FIREBASE_AUTH "3X1xpvWqW8fU8bzgls3IXZPBJFt7n401r6ouIIRa"

#define WIFI_SSID "Raj" 
#define WIFI_PASSWORD "raj@1999"
#define DHTPIN D5 
#define DHTTYPE DHT11
String pathTemp = "/Temperature"; 
String pathHum = "/Humidity"; 

DHT dht (DHTPIN, DHTTYPE);
FirebaseData firebaseData;
void setup () {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny"); 
  dht.begin (); 
}
void loop () 
{
  float h = dht.readHumidity (); 
  float t = dht.readTemperature (); 
  
  Firebase.pushFloat(firebaseData,pathTemp,t);
  Firebase.pushFloat(firebaseData,pathHum,h);  
  if (isnan (h) || isnan (t)) {
    Serial.println ("Failed to read from DHT sensor!");
    return;
  }  
  Serial.print ("Humidity: "); Serial.print (h);
  String firebaseHumid = String (h) + String ("%"); 
  Serial.print ("% ; Temperature: "); Serial.print (t); Serial.println ("°C");
  String firebaseTemp = String (t) + String ("°C"); 
  delay (5000);
  
//  Firebase.pushString ("pathHum", firebaseHumid); 
//  Firebase.pushString ("pathTemp", firebaseTemp);   
//   Firebase.setString("iotcontroller-9e3c2/Humidity", firebaseHumid);
//   Firebase.setString("/iotcontroller-9e3c2/Temperature", firebaseTemp);
}
