#include "sensors.h"

/*--------------------------------*/
void setup_led(int LEDpin, int mode, int status){
  pinMode(LEDpin, mode);
  digitalWrite(LEDpin, status);// Set outputs to LOW
}

/*--------------------------------*/
String get_temperature(DallasTemperature tempSensor) {
  /* Renvoie la valeur du capteur de temperature dans une String
   * Attention !!
   *    J'ai enleve le delay mais convertir prend du temps ! 
   *    moins que les requetes Http.
   */
  float t;
  String s;
  tempSensor.requestTemperaturesByIndex(0);
  t = tempSensor.getTempCByIndex(0);
  s = String(t);
  Serial.println("Temperature : "+s+" C"); // for debug
  return s;
}
/*--------------------------------*/
String get_light(int LightPin) {
  /* Renvoie la valeur du capteur de lumiere dans une String
   */
  int sensorValue;
  sensorValue = analogRead(LightPin);
  String s = String(sensorValue);
  return s;
}

String get_location(float lat, float lgn) {
  return "" + String(lat) + ", " + String(lgn) + "";
}

String getSSID() {
  return String(WiFi.SSID());
}

String getMAC() {
  return String(WiFi.macAddress());
}

String getIP() {
  return WiFi.localIP().toString();
}


String get_cooler_str(bool cool) {
  if (cool) {
    return "ON";
  } else {
    return "OFF";
  }
}


String get_heater_str(bool heat) {
    if (heat) {
    return "ON";
  } else {
    return "OFF";
  }
}

/*--------------------------------*/
float get_temperature_float(DallasTemperature tempSensor) {
  /* Renvoie la valeur du capteur de temperature dans une String
   * Attention !!
   *    J'ai enleve le delay mais convertir prend du temps ! 
   *    moins que les requetes Http.
   */
  float t;
  String s;
  tempSensor.requestTemperaturesByIndex(0);
  t = tempSensor.getTempCByIndex(0);
  return t;
}


/*--------------------------------*/
int get_light_int(int LightPin) {
  /* Renvoie la valeur du capteur de lumiere dans une String
   */
  int sensorValue;
  sensorValue = analogRead(LightPin);
  return sensorValue;
}
