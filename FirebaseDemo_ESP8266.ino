#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

#define FIREBASE_HOST "charming-aegis-855.firebaseio.com" //host firebase real-time database
#define FIREBASE_AUTH "4YfBl0xEkAKnMAjAwibwB4A4xYXbwwyuw3R4JRMX" //secrets db
#define WIFI_SSID "20 Laboratory" //ssid wifi
#define WIFI_PASSWORD "mg;o9uhld^Ux" //paswword wifi
#define DHTPIN 2 //pin input sensor
#define DHTTYPE DHT11 //type sensor 
DHT dht(DHTPIN, DHTTYPE); //init value

const String deviceName = "EspressoLiteV2";

void setup() {
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString(deviceName + "/macAddress", WiFi.macAddress());
}

void loop() {
  // handle error
  if (Firebase.failed()) {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    return;
  }

  //read DHT sensor very 2sec
  float h = 0.00f;
  float t = 0.00f;
  h = dht.readHumidity();
  t = dht.readTemperature();
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& valueObject = jsonBuffer.createObject();
  valueObject["temperature"] = (String) t;
  valueObject["humidity"] = (String) h;

  //push|set data to real-time db
  Firebase.set(deviceName + "/value", valueObject);
  delay(2000);
}
