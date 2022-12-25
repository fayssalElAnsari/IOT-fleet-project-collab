#ifndef CONSTANTS 
#define CONSTATNS

#define SaveDisconnectTime 1000 // Connection seems to need several tries (often two)
// Time in ms for save disconnection, => delay between try
// cf https://github.com/espressif/arduino-esp32/issues/2501  
#define WiFiMaxTry 10

/**
 * features toggle 
 */
bool http_feature = false;
bool mqtt_feature = true;

/* ---- Set timer ---- */
unsigned long loop_period = 1L * 1000; /* =>  10000ms : 10 s */

/** We define a max and min temperatures 
 *  so that we can turn on the FAN when 
 *  the temperature exceeds MAX_TEMP
 *  and turn on the heater (non-existent)
 *  when the temperature gets below MIN_TEMP
 */
const int MAX_TEMP = 28;
const int MIN_TEMP = 25;

const int CoolerLED = 19;
const int HeaterLED = 21;
const int TempSensor = 23;
const int Fan = 27; // no fan
const int Tachometer = 26; // no tach
const int LightSensor = 33; // Read analog input on ADC1_CHANNEL_5 (GPIO 33)
const int FireLED = 2;
float t;// temp digit
int l;
int lastTime = 0;

/**
 * these are identifiers of the state
 * ofthe ESP32, when the ESP32 is on
 * The StateLED is also on
 */
String StateLED = "off";
String IDENTIFIER = "Fayssal_ESP32";
String LOCATION;
String UPTIME;
const char* hostname = "Fayssal_ESP32"; /* FOR OTA */

/* ---- Light ----*/
const int LightPin = A5; // Read analog input on ADC1_CHANNEL_5 (GPIO 33)

/**
 * 
 */
bool RUNNING;
bool HEAT = false;
bool COOL = false;
bool FIRE = false;

const float lat = 36.36;
const float lgn = 25.25;

/**
 * general textual bool values to use later on ;D
 */
const bool ON = true;
const bool OFF = false;
const bool NO = false;
const bool YES = true;

int led1 = OFF;

/*===========================================*/

short int Light_threshold = 250; // Less => night, more => day

// Host for periodic data report
  /** DEFAULTS **/
String TARGET_IP = "192.168.101.5";
int TARGET_PORT = 1880;
int TARGET_REFRESH = 5000;
  /** VARS **/
String target_ip = TARGET_IP;
int target_port = TARGET_PORT;
int target_sp = TARGET_REFRESH; // post broadcast intervall

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

/* ==== MQTT BROKER/SERVER ==== */
//const char* mqtt_server = "192.168.101.5"; // this computer's local IP
//const char* mqtt_server = "127.0.0.1";
//const char* mqtt_server = "broker.hivemq.com"; 
//const char* mqtt_server = "iot.eclipse.org";
const char* mqtt_server = "test.mosquitto.org";
/* ==== MQTT TOPICS ==== */
#define TOPIC_TP "mca/M1/iot"
#define TOPIC_TEMP "fayssal/el/ansari/sensors/temp"
#define TOPIC_LED "fayssal/el/ansari/sensors/led"
#define TOPIC_JSONPAYLOAD "fayssal/el/ansari/jsonpayload"
#define TOPIC_LOCATION "fayssal/el/ansari/sensors/loc"
// MQTT
WiFiClient wifiClient;
PubSubClient pubSubClient;

/* ---- TEMP ---- */
OneWire oneWire(23); // Pour utiliser une entite oneWire sur le port 23
DallasTemperature tempSensor(&oneWire) ; // Cette entite est utilisee
           // par le capteur de
           // temperature
           

#endif 
