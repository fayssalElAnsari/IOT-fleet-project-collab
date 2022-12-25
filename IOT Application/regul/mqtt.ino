#include "mqtt.h"

const char* username = "FAYSSAL";
const char* password = "PASSWORD";

/* ============== SUBSCRIBE ============= */
void mqtt_mysubscribe(char* topic) {
  /* subscribe to MQTT topic */
  while (!pubSubClient.connected()) { // Loop untill we're connected
    Serial.print("Attempting MQTT connection...");
    if (pubSubClient.connect("esp32", 
                              username,
                              password)) {
      Serial.println("connected");
      // then subscribe topic                      
      } else {
        Serial.print("failed, rc=");
        Serial.print(pubSubClient.state());

        Serial.println(" try again in 5 seconds");
        delay(5000);// wait 5s before retry 
      }
  }
}

/*============== CALLBACK ===================*/
void mqtt_pubcallback(char* topic, byte* message, unsigned int length) {
  Serial.print("A new message has been published on topic: ");
  Serial.println(topic);
  Serial.print(" => ");

  // convert msg to String and print
  String messageString;
  for(int i =0; i < length; i++) {
    Serial.print((char)message[i]);
    messageString += (char) message[i];
  }
  Serial.println();

  /**
   * the msg should be a string of the form "lat, lon"
   * example: -m "36.5, 24.5"
   * this will update the current location variable to the given string
   * and the new location will be sent in the upcoming JSON mqtt broadcasts
   */
  if(String(topic) == TOPIC_LED) {
    Serial.print("[*] The LED is now set to ");
    if( messageString == "on") {
      Serial.println("on");
//        set_LED(HIGH); // TODO
    } else if (messageString == "off") {
      Serial.println("off");
//        set_LED(LOW); // TODO
    }
  } else if (String(topic) == TOPIC_LOCATION) {
    Serial.print("[*] The LOCATION is now set to ");
    Serial.println(messageString);
    LOCATION = messageString;
  }
}
