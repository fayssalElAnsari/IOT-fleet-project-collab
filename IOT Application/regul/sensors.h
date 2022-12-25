#ifndef SENSORS
#define SENSORS

#include "OneWire.h"
#include "DallasTemperature.h"

void setup_led(int LEDpin, int mode, int status);

String get_temperature(DallasTemperature tempSensor);
float get_temperature_float(DallasTemperature tempSensor);
String get_light(int LightPin);
int get_light_int(int LightPin);
String get_location(float lat, float lgn);

#endif
