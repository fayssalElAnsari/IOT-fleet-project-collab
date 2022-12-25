#include "utils.h"

String getJSONString_fromstatus(float temp, int light){
  /*
   * put all relevant data from esp in a "json formatted" String
   */
  StaticJsonDocument<1000> jsondoc;
  jsondoc["status"]["temperature"] = temp; // Temp value
  jsondoc["status"]["light"] = light; // Light value
  jsondoc["status"]["heat"] = HEAT; // Heater
  jsondoc["status"]["cool"] = COOL; // Cooler
  jsondoc["status"]["running"] = RUNNING; // Regulation status : RUNNING or HALT
  jsondoc["status"]["fire"] = getFire(); // NO or YES   

  jsondoc["regul"]["sh"] = HIGH; // Seuil Haut
  jsondoc["regul"]["sb"] = LOW;  // Seuil bas

  jsondoc["info"]["loc"] = LOCATION;
  jsondoc["info"]["user"] = IDENTIFIER;
  jsondoc["info"]["uptime"] = getUptime();
  jsondoc["info"]["ssid"] = getSSID(); // utile quand on se sera connecté .. pour l'instant inutile
  jsondoc["info"]["ident"] = getMAC(); // utile quand on se sera connecté .. pour l'instant inutile
  jsondoc["info"]["ip"] = getIP(); // utile quand on se sera connecté .. pour l'instant inutile
 

//  jsondoc["reporthost"]["target_ip"] = target_ip; // utile quand on se sera connecté .. pour l'instant inutile
//  jsondoc["reporthost"]["target_port"] = target_port; // utile quand on se sera connecté .. pour l'instant inutile
//  jsondoc["reporthost"]["sp"] = target_sp; // utile quand on se sera connecté .. pour l'instant inutile
 
  String data = "";
  serializeJson(jsondoc, data);
  return data;
}

String get_t(){
  return String(t);
}

String getUptime() {
  uptime::calculateUptime();
  UPTIME = String(uptime::getMinutes()) +"m :  " + String(uptime::getSeconds()) + "s ";
  return UPTIME;
}

String getJSONString_fromlocation(float lat, float lgn){
  StaticJsonDocument<1000> jsondoc;
  jsondoc["lat"] = lat;
  jsondoc["lgn"] = lgn;
  String data = "";
  serializeJson(jsondoc, data);
  return data;
}

int getFire(){
  return FIRE;
}

bool switch_cooler(bool cooler_on){
  if (cooler_on){
    digitalWrite(CoolerLED, HIGH);
    COOL = ON;
  } else {
    digitalWrite(CoolerLED, LOW);
    COOL = OFF;
  }
  return COOL;
}


bool switch_heater(bool heater_on) {
  if (heater_on){
    digitalWrite(HeaterLED, HIGH);
    HEAT = ON;
  } else {
    digitalWrite(HeaterLED, LOW);
    HEAT = OFF;
  }
  return HEAT;
}

/*
 * redefine the delay(int) method in a non async manner to avoid crash
 */
void delay(int duration) {
    lastTime = millis();
    Serial.println("delaying.");
    while ((millis() - lastTime) < duration) {
//      Serial.print(".");
    }
}
