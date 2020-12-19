#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include </usr/include/json-c/json.h>

using namespace std;

#define WEATHER_API_KEY_FILE "weather_key.txt"
#define LOCATION_API_KEY_FILE "location_key.txt"
#define DATA_FILE "data.json"

void splash(void);
void wb_get_ambience(int mode);
void wb_get_ambience_mode(void);
void wb_set_unit_sys(void);
void wb_auto_locate(void);
void get_location_data(void);
void wb_parse_ambience(int mode);
