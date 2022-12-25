/*** Basic/Static Wifi connection

     Fichier wificonnect/wifi_utils.h ***/

#include <WiFi.h> // https://www.arduino.cc/en/Reference/WiFi
#include <WiFiMulti.h>

#ifndef WIFIUTILSH
#define WIFIUTILSH
#define SaveDisconnectTime 1000 // Connection seems to need several tries (often two)
// Time in ms for save disconnection, => delay between try
// cf https://github.com/espressif/arduino-esp32/issues/2501  
#define WiFiMaxTry 10
#endif


String translateEncryptionType(wifi_auth_mode_t encryptionType);
void wifi_status();
void wifi_connect_basic(String hostname, String ssid, String passwd);
bool wifi_connect_multi(String hostname);
