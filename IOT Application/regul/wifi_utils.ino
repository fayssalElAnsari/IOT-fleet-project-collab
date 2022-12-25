/*** Basic/Static Wifi connection

     Fichier wificonnect/wifi_utils.ino ***/

#include <WiFi.h> // https://www.arduino.cc/en/Reference/WiFi
#include "wifi_utils.h"
/*--------------------------------------------------------------------------*/

String translateEncryptionType(wifi_auth_mode_t encryptionType) {
  // cf https://www.arduino.cc/en/Reference/WiFiEncryptionType 
  switch (encryptionType) {
  case (WIFI_AUTH_OPEN):
    return "Open";
  case (WIFI_AUTH_WEP):
    return "WEP";
  case (WIFI_AUTH_WPA_PSK):
    return "WPA_PSK";
  case (WIFI_AUTH_WPA2_PSK):
    return "WPA2_PSK";
  case (WIFI_AUTH_WPA_WPA2_PSK):
    return "WPA_WPA2_PSK";
  case (WIFI_AUTH_WPA2_ENTERPRISE):
    return "WPA2_ENTERPRISE";
  }
}

/*--------------------------------------------------------------------------*/
void wifi_status(){
  /* print the status of the connected wifi  : two ways ! */
  // Use Pure C =>  array of chars
  //Serial.printf("WiFi Status : \n");
  //Serial.printf("\t#%d\n", i);
  //Serial.printf("\tIP address : %s\n", WiFi.localIP().toString().c_str());
  //Serial.printf("\tMAC address : %s\n", WiFi.macAddress().c_str());
  //Serial.printf("\tSSID : %s\n", WiFi.SSID());
  //Serial.printf("\tReceived Signal Strength Indication : %ld dBm\n",WiFi.RSSI());
  //Serial.printf("\tReceived Signal Strength Indication : %ld %\n",constrain(2 * (WiFi.RSSI() + 100), 0, 100));
  //Serial.printf("\tBSSID : %s\n", WiFi.BSSIDstr().c_str());
  //Serial.printf("\tEncryption type : %s\n", translateEncryptionType(WiFi.encryptionType(0)));
  
  // Use of C++ =>  String !
  String s = "WiFi Status : \n";
  //s += "\t#" + String() + "\n";
  s += "\tIP address : " + WiFi.localIP().toString() + "\n"; 
  s += "\tMAC address : " + String(WiFi.macAddress()) + "\n";
  s += "\tSSID : " + String(WiFi.SSID()) + "\n";
  s += "\tReceived Signal Strength Indication : " + String(WiFi.RSSI()) + " dBm\n";
  s += "\tReceived Signal Strength Indication : " + String(constrain(2 * (WiFi.RSSI() + 100), 0, 100)) + " %\n";
  s += "\tBSSID : " + String(WiFi.BSSIDstr()) + "\n";
  s += "\tEncryption type : " + translateEncryptionType(WiFi.encryptionType(0))+ "\n";
  Serial.print(s);
}




/*--------------------------------------------------------------------------*/
void wifi_connect_basic(String hostname, String ssid, String passwd){
  int nbtry = 0; // Nb of try to connect

  WiFi.mode(WIFI_OFF);   
  WiFi.mode(WIFI_STA); // Set WiFi to station mode 
  // delete old config
  // WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.disconnect(true); // Disconnect from an AP if it was previously connected
  // WiFi.persistent(false); // Avoid to store Wifi configuration in Flash
  
  // Define hostname before begin => in C str ! not C++
  WiFi.setHostname(hostname.c_str()); 
  
  Serial.printf("\nAttempting %d to connect AP of SSID : %s", nbtry, ssid.c_str());
  WiFi.begin(ssid.c_str(), passwd.c_str());
  //WiFi.begin(ssid.c_str(), passwd.c_str(), 0, WiFi.BSSID(thegoodone));
  while(WiFi.status() != WL_CONNECTED && (nbtry < WiFiMaxTry)){
//    delay(SaveDisconnectTime); // 500ms seems to work in most cases, may depend on AP
    Serial.print(".");
    nbtry++;
  }
}

/*--------------------------------------------------------------------------*/
bool wifi_connect_multi(String hostname){
  
  WiFiMulti wm; // Creates an instance of the WiFiMulti class
  // Attention ! PAS arrivé à sortir l'instance de la fonction => heap error ! Why ???? 
  wm.addAP("_FAYSSAL","qwertyui");
  wm.addAP("HUAWEI-6EC2", "FGY9MLBL");
  wm.addAP("HUAWEI-553A", "QTM06RTT");
  wm.addAP("GMAP", "vijx47050");
  wm.addAP("Livebox-B870","MYCNcZqnvsWsiy7s52");

  WiFi.mode(WIFI_OFF);   
  WiFi.mode(WIFI_STA); // Set WiFi to station mode 
  // delete old config
  // WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.disconnect(true); // Disconnect from an AP if it was previously connected
  // WiFi.persistent(false); // Avoid to store Wifi configuration in Flash
  
  // Define hostname  => in C str ! not C++
  WiFi.setHostname(hostname.c_str());
  
  Serial.println(String("\nAttempting to connect to SSIDs : "));
  while(wm.run() != WL_CONNECTED) {
    delay(SaveDisconnectTime);
    Serial.print(".");
  }
  
  if(wm.run() == WL_CONNECTED) {
    Serial.print("wifiMulti connected : Yes !");
    return true;
  }

  return false;
}
