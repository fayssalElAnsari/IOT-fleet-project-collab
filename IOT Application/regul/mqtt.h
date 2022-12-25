#ifndef MQTT
#define MQTT

/* ============== SUBSCRIBE ============= */
void mqtt_mysubscribe(char* topic);

/*============== CALLBACK ===================*/
void mqtt_pubcallback(char* topic, byte* message, unsigned int length);

#endif
