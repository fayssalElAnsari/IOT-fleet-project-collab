#ifndef UTILS
#define UTILS

String getJSONString_fromstatus(float temp, int light);

String get_t();

String getUptime();

String getJSONString_fromlocation(float lat, float lgn);

int getFire();

bool switch_cooler(bool cooler_on);

bool switch_heater(bool heater_on);

void delay(float duration);

#endif
