#ifndef EEPROM_FEATURES_H
#define EEPROM_FEATURES_H

#include <Arduino.h>
#include <EEPROM.h>  // Include EEPROM library
#include "global_variables.hpp" // Include the variables

#define EEPROM_SIZE 256

#define ADDR_UI_NETWORK 0
#define ADDR_UI_SIGNAL 7
#define ADDR_UI_STATUS_MQTT 13
#define ADDR_NETWORK_APN 23
#define ADDR_MQTT_SERVER 33
#define ADDR_MQTT_USER 43
#define ADDR_MQTT_PW 50
#define ADDR_MQTT_DN 60
#define ADDR_MQTT_ID 72
#define ADDR_MQTT_SD 86
#define ADDR_MONDAY_ON_TIME 100
#define ADDR_MONDAY_OFF_TIME 102
#define ADDR_TUESDAY_ON_TIME 104
#define ADDR_TUESDAY_OFF_TIME 106
#define ADDR_WEDNESDAY_ON_TIME 108
#define ADDR_WEDNESDAY_OFF_TIME 110
#define ADDR_THURSDAY_ON_TIME 112
#define ADDR_THURSDAY_OFF_TIME 114
#define ADDR_FRIDAY_ON_TIME 116
#define ADDR_FRIDAY_OFF_TIME 118
#define ADDR_SATURDAY_ON_TIME 120
#define ADDR_SATURDAY_OFF_TIME 122
#define ADDR_SUNDAY_ON_TIME 124
#define ADDR_SUNDAY_OFF_TIME 126
#define ADDR_D1_ON_TIME 128
#define ADDR_D1_OFF_TIME 130
#define ADDR_D2_ON_TIME 132
#define ADDR_D2_OFF_TIME 134
#define ADDR_D3_ON_TIME 136
#define ADDR_D3_OFF_TIME 138
#define ADDR_D4_ON_TIME 140
#define ADDR_D4_OFF_TIME 142
#define ADDR_D5_ON_TIME 144
#define ADDR_D5_OFF_TIME 146
#define ADDR_DAY1 156
#define ADDR_DAY2 166
#define ADDR_DAY3 176
#define ADDR_DAY4 186
#define ADDR_DAY5 196
#define ADDR_DAY6 106


void EEPROM_RESET_CHECK();
void GetLastSaveValues();
void writeString(int startingAddress, String data);
String readString(int startingAddress, int length);
void ResetParam();
void WeekdayUpdate();
void SpecialDayUpdate();
Time parseTime(const String& timeStr);
Date parseDate(const String& dateStr);

#endif // EEPROM_FUNCTIONS_H
