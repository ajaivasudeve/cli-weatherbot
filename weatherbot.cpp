#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <curl/curl.h>
#include <curses.h>
#include <algorithm>
#include <ctype.h>
#include "country_codes.h"
#include </usr/include/json-c/json.h>

using namespace std;

#define clearscreen() printf("\033[H\033[J")
#define WEATHER_FILE "weather.json"
#define API_KEY_FILE "key.txt"

static string api_key = "0";
static string unit_string = "standard";
static string temp_unit = " K";
static string speed_unit = " m/s";

void splash()
{
    clearscreen();
    cout << "\n*--------------------------------------------*\n";
    cout << "|--------------------------------------------|\n";
    cout << "|--------------------------------------------|\n";
    cout << "|-----------------WeatherBot-----------------|\n";
    cout << "|--------------------------------------------|\n";
    cout << "|--------------------------------------------|\n";
    cout << "*--------------------------------------------*\n";
    cout << "\n";
}

int parse_weather_json()
{
    char buffer[2048];
    FILE *fp;

    struct json_object *parsed_json;
    struct json_object *name;
    struct json_object *weather;
    struct json_object *weather_desc;
    struct json_object *condition;
    struct json_object *main;
    struct json_object *temp;
    struct json_object *feels_like;
    struct json_object *temp_min;
    struct json_object *temp_max;
    struct json_object *pressure;
    struct json_object *humidity;
    struct json_object *visibility;
    struct json_object *wind;
    struct json_object *speed;
    struct json_object *deg;

    fp = fopen(WEATHER_FILE, "r");
    fread(buffer, 2048, 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);

    json_object_object_get_ex(parsed_json, "name", &name);
    json_object_object_get_ex(parsed_json, "main", &main);
    json_object_object_get_ex(parsed_json, "weather", &weather);
    json_object_object_get_ex(parsed_json, "visibility", &visibility);
    json_object_object_get_ex(parsed_json, "wind", &wind);
    json_object_object_get_ex(wind, "speed", &speed);
    json_object_object_get_ex(wind, "deg", &deg);
    json_object_object_get_ex(main, "temp", &temp);
    json_object_object_get_ex(main, "feels_like", &feels_like);
    json_object_object_get_ex(main, "temp_min", &temp_min);
    json_object_object_get_ex(main, "temp_max", &temp_max);
    json_object_object_get_ex(main, "pressure", &pressure);
    json_object_object_get_ex(main, "humidity", &humidity);
    json_object_object_get_ex(parsed_json, "visibility", &visibility);

    weather_desc = json_object_array_get_idx(weather, 0);
    json_object_object_get_ex(weather_desc, "main", &condition);

    splash();
    cout << "\033[0;30;47m"
         << "Weather Data             : "
         << "Value\033[0m" << endl;
    cout << "Place                    : " << json_object_get_string(name) << endl;
    cout << "Condition                : " << json_object_get_string(condition) << endl;
    cout << "Current Temperature      : " << json_object_get_int(temp) << temp_unit << endl;
    cout << "Feels Like               : " << json_object_get_int(feels_like) << temp_unit << endl;
    cout << "Min Temp                 : " << json_object_get_int(temp_min) << temp_unit << endl;
    cout << "Max Temp                 : " << json_object_get_int(temp_max) << temp_unit << endl;
    cout << "Pressure                 : " << json_object_get_int(pressure) << " hPa" << endl;
    cout << "Humidity                 : " << json_object_get_int(humidity) << "%" << endl;
    if (unit_string == "Imperial")
        cout << "Visibility               : " << json_object_get_int(visibility) * 3.28084 << " ft" << endl;
    else
        cout << "Visibility               : " << json_object_get_int(visibility) << " m" << endl;
    cout << "Wind Speed               : " << json_object_get_int(speed) << speed_unit << endl;
    cout << "Wind Direction           : " << json_object_get_int(deg) << "°" << endl;
    cout << "\nPress Any Key to Return";

    getchar();

    return 0;
}

int get_preferred_unit_sys()
{
    int selection;

    splash();
unit_menu:
    cout << "\033[0;30;47mSelect Unit System:\033[0m\n"
            "\t1. Standard [Temp: Kelvin, Speed: Meter per Second]\n"
            "\t2. Metric   [Temp: °Celcius, Speed: Meter per Second]\n"
            "\t3. Imperial [Temp: °Fahrenheit, Speed: Miles per Hour]\n"
            "\t4. Go Back\n"
            "\n"
            "Your Selection: ";
    cin >> selection;
    cin.clear();
    cin.ignore();

    switch (selection)
    {
    case 1:
        unit_string = "standard";
        temp_unit = " K";
        speed_unit = " m/s";
        splash();
        unit_string[0] = toupper(unit_string[0]);
        cout << "\033[0;37;42m"
             << "Message: Set unit system - " << unit_string << "\033[0m" << endl;
        cout << "\n";
        goto unit_menu;
        break;
    case 2:
        unit_string = "metric";
        temp_unit = "° C";
        speed_unit = " m/s";
        splash();
        unit_string[0] = toupper(unit_string[0]);
        cout << "\033[0;37;42m"
             << "Message: Set unit system - " << unit_string << "\033[0m" << endl;
        cout << "\n";
        goto unit_menu;
        break;
    case 3:
        unit_string = "imperial";
        temp_unit = "° F";
        speed_unit = " MPH";
        splash();
        unit_string[0] = toupper(unit_string[0]);
        cout << "\033[0;37;42m"
             << "Message: Set unit system - " << unit_string << "\033[0m" << endl;
        cout << "\n";
        goto unit_menu;
        break;
    case 4:
        splash();
        break;
    default:
        splash();
        cout << "\033[0;37;41mError: Selection Invalid!\033[0m\n";
        cout << "\n";
        goto unit_menu;
        break;
    }

    return 0;
}

string get_country_code()
{
    string country_code;
    string input;
    char country_init, conv_country_init;

    splash();
init_entry:
    cout << "Enter the first letter of your Country name: ";
    cin >> input;
    cin.clear();
    cin.ignore();

    country_init = input[0];

    splash();
    conv_country_init = toupper(country_init);
    cout << "\033[0;30;47m"
         << "Country Name                            Country Code\033[0m\n";
    switch (conv_country_init)
    {
    case 'A':
        cout << COUNTRY_INIT_A;
        break;
    case 'B':
        cout << COUNTRY_INIT_B;
        break;
    case 'C':
        cout << COUNTRY_INIT_C;
        break;
    case 'D':
        cout << COUNTRY_INIT_D;
        break;
    case 'E':
        cout << COUNTRY_INIT_E;
        break;
    case 'F':
        cout << COUNTRY_INIT_F;
        break;
    case 'G':
        cout << COUNTRY_INIT_G;
        break;
    case 'H':
        cout << COUNTRY_INIT_H;
        break;
    case 'I':
        cout << COUNTRY_INIT_I;
        break;
    case 'J':
        cout << COUNTRY_INIT_J;
        break;
    case 'K':
        cout << COUNTRY_INIT_K;
        break;
    case 'L':
        cout << COUNTRY_INIT_L;
        break;
    case 'M':
        cout << COUNTRY_INIT_M;
        break;
    case 'N':
        cout << COUNTRY_INIT_N;
        break;
    case 'O':
        cout << COUNTRY_INIT_O;
        break;
    case 'P':
        cout << COUNTRY_INIT_P;
        break;
    case 'Q':
        cout << COUNTRY_INIT_Q;
        break;
    case 'R':
        cout << COUNTRY_INIT_R;
        break;
    case 'S':
        cout << COUNTRY_INIT_S;
        break;
    case 'T':
        cout << COUNTRY_INIT_T;
        break;
    case 'U':
        cout << COUNTRY_INIT_U;
        break;
    case 'V':
        cout << COUNTRY_INIT_V;
        break;
    case 'W':
        cout << COUNTRY_INIT_W;
        break;
    case 'X':
        cout << COUNTRY_INIT_X;
        break;
    case 'Y':
        cout << COUNTRY_INIT_Y;
        break;
    case 'Z':
        cout << COUNTRY_INIT_Z;
        break;
    default:
        splash();
        cout << "\033[0;37;41mError: Entry Invalid!\033[0m\n";
        cout << "\n";
        goto init_entry;
        break;
    }

    cout << "\nEnter Code corresponding to your Country: ";
    getline(cin, country_code);

    transform(country_code.begin(), country_code.end(), country_code.begin(), ::toupper);

    return country_code;
}

int fetch_weather_info_place()
{
    CURL *curl;
    FILE *fp;
    long result;
    int selection;
    string name;
    string url;

    splash();
enter_name:
    cout << "\033[0;30;47mType 'EXIT' to Go Back\033[0m\n";
    cout << "\nEnter Location Name: ";
    getline(cin, name);

    name[0] = tolower(name[0]);

    if (name == "exit")
        return 0;

    url += "http://api.openweathermap.org/data/2.5/weather?q=" + name + "&appid=" + api_key + "&units=" + unit_string;

    curl = curl_easy_init();
    fp = fopen(WEATHER_FILE, "wb");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    result = curl_easy_perform(curl);

    fclose(fp);
    curl_easy_cleanup(curl);

    if (result == CURLE_OK)
    {
        parse_weather_json();
        return 0;
    }
    else
    {
        splash();
        cout << "\033[0;37;41mError: Failed to fetch weather info!\033[0m\n";
        cout << "\n";
        goto enter_name;
    }

    return 0;
}

int fetch_weather_info_pin()
{
    CURL *curl;
    FILE *fp;
    long result;
    int selection;
    string pin;
    string country_code;
    string url;

    splash();
weather_menu:
    cout << "\033[0;30;47mDefault Country: IN - India\033[0m\n"
            "\t1. Continue\n"
            "\t2. Change Country\n"
            "\t3. Go Back\n"
            "\n"
            "Your Selection: ";
    cin >> selection;
    cin.clear();
    cin.ignore();

    switch (selection)
    {
    case 1:
        country_code = "IN";
        break;
    case 2:
        country_code = get_country_code();
        break;
    case 3:
        splash();
        return 0;
        break;
    default:
        splash();
        cout << "\033[0;37;41mError: Selection Invalid!\033[0m\n";
        cout << "\n";
        goto weather_menu;
        break;
    }

    splash();
    cout << "Enter postal index code of the location: ";
    getline(cin, pin);

    url += "http://api.openweathermap.org/data/2.5/weather?zip=" + pin + "," + country_code + "&appid=" + api_key + "&units=" + unit_string;

    curl = curl_easy_init();
    fp = fopen(WEATHER_FILE, "wb");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    result = curl_easy_perform(curl);

    fclose(fp);
    curl_easy_cleanup(curl);

    if (result == CURLE_OK)
    {
        parse_weather_json();
        return 0;
    }
    else
    {
        splash();
        cout << "\033[0;37;41mError: Failed to fetch weather info!\033[0m\n";
        cout << "\n";
        goto weather_menu;
    }

    return 0;
}

void load_api_key()
{
    ifstream myfile(API_KEY_FILE);
    if (myfile.is_open())
    {
        getline(myfile, api_key);
        myfile.close();
    }
}

int main()
{
    int selection;

    splash();

    load_api_key();

    if (api_key == "0")
    {
        cout << "\033[0;37;41mError: API Key Not Found!\033[0m\n";
        cout << "\nPress Any Key to Exit: ";
        getchar();
        return 0;
    }

main_menu:
    cout << "\033[0;30;47mMain Menu:\033[0m\n"
            "\t1. Check Weather by Place Name\n"
            "\t2. Check Weather by Postal Code\n"
            "\t3. Select preferred Unit System\n"
            "\t4. Quit\n"
            "\n"
            "Enter Your Selection: ";
    cin >> selection;
    cin.clear();
    cin.ignore();

    switch (selection)
    {
    case 1:
        fetch_weather_info_place();
        splash();
        goto main_menu;
        break;
    case 2:
        fetch_weather_info_pin();
        splash();
        goto main_menu;
        break;
    case 3:
        get_preferred_unit_sys();
        goto main_menu;
        break;
    case 4:
        break;
    default:
        splash();
        cout << "\033[0;37;41mError: Selection Invalid!\033[0m\n";
        cout << "\n";
        goto main_menu;
        break;
    }
}