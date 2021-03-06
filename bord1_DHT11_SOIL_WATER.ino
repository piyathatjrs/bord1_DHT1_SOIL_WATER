#include "DHT.h"
#include <WiFi.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include<PubSubClient.h>

#define DHTPIN 04
#define DHTTYPE DHT11
  
#define WIFI_SSD "Scotch" 
#define WIFI_PASS "1231231239"

#define MQTT_SERVER   "tailor.cloudmqtt.com"
#define MQTT_PORT       14090
#define MQTT_USERNAME "zpfdcyrx"
#define MQTT_PASSWORD "OypjtCmtYhqp"
#define MQTT_NAME     "ESSSSS"

WiFiClient client;

PubSubClient mqtt(client);




void callback(char* topic, byte* payload, unsigned int length) {
 String topic_str = "";
 String payload_str = "";
 for(int j=0 ; j<strlen(topic) ;j++){
  topic_str += topic[j];
  }
  for(int i=0 ; i<length ; i++){
    Serial.print((char)payload[i]);
    payload_str += (char)payload[i]; 
    }

    Serial.print("[ ");
    Serial.print(topic);
    Serial.print(" ]");
    Serial.print(payload_str);
    
}

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSD);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSD, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  digitalWrite(LED_BUILTIN, LOW);
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);
 
  connectMQTT();
}

//water
const int AirValue = 0;   //you need to replace this value with Value_1
const int WaterValue = 2600;  //you need to replace this value with Value_2
int sensorValue = analogRead(32); 

//soil
  const int AirValue_soil = 620;   //you need to replace this value with Value_1
  const int WaterValue_soil = 310;  //you need to replace this value with Value_2




void loop() {
  // put your main code here, to run repeatedly:
   delay(1500);
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println();
      Publish("TEST/MQTT_DHT11", String(t));
delay(1500);
//water
int sensorValue_water = analogRead(32);
int map_value_water=0;
Serial.println(sensorValue_water) ;
map_value_water = map(sensorValue_water, AirValue, WaterValue, 0, 100);  
if(sensorValue_water >=2600){
   Serial.println(sensorValue_water); 
   Serial.println("100"); 
    Publish("TEST/MQTT_WATER", ("100"));
  }  else if(sensorValue_water <=0 ){
     Serial.println(sensorValue_water); 
   Serial.println("0"); 
    Publish("TEST/MQTT_WATER", ("0"));
  } else {
    Serial.println(sensorValue_water); 
    Serial.print(map_value_water);
     Publish("TEST/MQTT_WATER", String(map_value_water));
    Serial.print(" %\n===============================\n");
    }
     
  Serial.println(); 
  delay(1500);

   
//   val = val/19.5;
//  Serial.println(val);
//    Publish("TEST/MQTT_WATER", String(val)); 
//    delay(1500);
//soil

        int soilMoistureValue = analogRead(33);  //put Sensor insert into soil
        soilMoistureValue = soilMoistureValue/4;
        int soilmoisturepercent=0;

   Serial.println(soilMoistureValue);
soilmoisturepercent = map(soilMoistureValue, AirValue_soil, WaterValue_soil, 0, 100);

Serial.println(soilmoisturepercent);
if(soilmoisturepercent >= 100)
{
  Serial.println("Soil = 100%"); 
  Publish("TEST/MQTT_SOIL", ("100"));
}
else if(soilmoisturepercent <=0)
{
  Serial.println("Soil = 0%");
   Publish("TEST/MQTT_SOIL", ("0"));
}
else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
{
   Serial.print(soilmoisturepercent);-
   Serial.println(" persen");
    Publish("TEST/MQTT_SOIL", String(soilmoisturepercent));
}
  delay(1500);
  
    //rain
    // read the input on analog pin 0:
//  int sensorValue = analogRead(35);
//  // print out the value you read:
//  sensorValue=100-(sensorValue/41);
//  Serial.println(sensorValue);
//    Publish("TEST/MQTT_RAIN", String(sensorValue)); 
//  delay(1500);  // delay in between reads for stability
  
    
  
}

void connectMQTT(){
  Serial.print("MQTT connect... ");
  if(mqtt.connect(MQTT_NAME ,MQTT_USERNAME , MQTT_PASSWORD )){
      Serial.print("success"); 
    }else {
      Serial.print("Failed");
      delay(5000);
      }
      Serial.println();

  }
    void Publish(String key , String message){
      char topic[100] , payload[100];
      key.toCharArray(topic,100);
      message.toCharArray(payload,100);
      mqtt.publish(topic , payload);
      }
      int analog_value = 0;
      double temp=0;

 

      
