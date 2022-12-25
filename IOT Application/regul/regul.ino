//#include "sensors.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "uptime_formatter.h"
#include "uptime.h"
#include "ota.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
//#include "AsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "utils.h"
#include <PubSubClient.h>
#include "mqtt.h"
#include "vars_constants.h"
#include "http_module.h"
  
/*---- Arduino IDE paradigm : setup+loop -----*/
void setup(){
  Serial.begin(9600);
  while (!Serial); // wait for a serial connection. Needed for native USB port only
  
  /* WiFi connection  -----------------------*/
  // Default Credentials 
  String ssid = String("_FAYSSAL");
  String passwd = String("qwertyui");
  bool connection_established = 0;
  wifi_connect_basic(hostname, ssid, passwd); // debug connect WIFI
//  connection_established = wifi_connect_multi(hostname); // reboot bug :(
//  while(!connection_established); // wait to connect to go to next line (to avoid reboot bug)
  wifi_status();

  /* ==== MQTT ==== */
  pubSubClient.setClient(wifiClient);
  pubSubClient.setServer(mqtt_server, 1883);
  pubSubClient.setCallback(mqtt_pubcallback);
  pubSubClient.setBufferSize(512);
  
  // Initialize the LED 
//  setup_led(LEDpin, OUTPUT, LOW);
  setup_led(CoolerLED, OUTPUT, LOW);
  setup_led(HeaterLED, OUTPUT, LOW);
  setup_led(FireLED, OUTPUT, LOW);


  LOCATION = "" + String(lat) + ", " + String(lgn) + "";
  // Init temperature sensor 
  tempSensor.begin();

  // status constants
  RUNNING = true;

  /* Install OTA --------------------------*/
//  setupOTA("Fayssal_ESP32"); // TODO: ... CONST CHAR

  // Initialize SPIFFS
  SPIFFS.begin(true);
  setup_http_server();
}


void loop(){  
  // update current location 
  // lat = getLat()
  // lgn = getLgn()
  // LOCATION = getJSONString_fromlocation(lat, lgn);
  
  t = get_temperature_float(tempSensor);
  l = get_light_int(LightSensor);

/**
 * regulate the temperature to stay between 
 * the min and max at all times
 */
  if (t > MIN_TEMP and t < MAX_TEMP){
    switch_heater(OFF);
    switch_cooler(OFF);
  }
  else if(t >= MAX_TEMP) {
    switch_heater(OFF);
    switch_cooler(ON);
  } else {
    switch_heater(ON);
    switch_cooler(OFF);
  }

  if(http_feature) {
    /* =============== HTTP-SERVER =============== */
    String serverPath = "http://" + target_ip + ":" + target_port + "/nodered";
    if ((millis() - lastTime) > target_sp) {    
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverPath.c_str());
      http.addHeader("Content-Type", "application/json");
      String payload = getJSONString_fromstatus(float(t),int(l)).c_str();
      Serial.println("sending post to" + serverPath + " :" + payload);
      int httpResponseCode = http.POST(payload);
      
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
      lastTime = millis();
    } 
  }

    /*--- subscribe to TOPIC_LED if not yet ! */
    if (!pubSubClient.connected()) {
      mqtt_mysubscribe((char *)(TOPIC_LED));
    } 
    /*--- subscribe to TOPIC_LOCATION if not yet ! */
  //  if (!pubSubClient.connected()) {
  //    mqtt_mysubscribe((char *)(TOPIC_LOCATION));
  //  } 

  if(mqtt_feature){
    /* =============== MQTT-SERVICE ============= */
    const char* jsonPayload = getJSONString_fromstatus(float(t), int(l)).c_str(); 
    pubSubClient.publish(TOPIC_TP, jsonPayload);
    Serial.print("published json object: ");
  
    /*--- subscribe to TOPIC_LED if not yet ! */
//    if (!pubSubClient.connected()) {
//      mqtt_mysubscribe((char *)(TOPIC_LED));
//    }
    /*--- subscribe to TOPIC_LOCATION if not yet ! */
    if (!pubSubClient.connected()) {
      mqtt_mysubscribe((char *)(TOPIC_LOCATION));
    } 
  }
  
  delay(5000);
  
}
