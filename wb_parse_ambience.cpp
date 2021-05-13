#include "weatherbot.h"

extern std::string label;
extern std::string unit_sys;
extern std::string temp_unit;
extern std::string speed_unit;

void wb_parse_weather(void)
{
    char buffer[2048];
    FILE *fp;

    struct json_object *parsed_json;
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

    fp = fopen(DATA_FILE, "r");
    fread(buffer, 2048, 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);

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

    if (json_object_get_type(weather) != json_type_array)
        return;
    weather_desc = json_object_array_get_idx(weather, 0);
    json_object_object_get_ex(weather_desc, "main", &condition);

    splash();
    cout << "\033[0;30;47m"
         << "Weather Data             : "
         << "Value\033[0m" << endl;
    cout << "Place                    : " << label << endl;
    cout << "Condition                : " << json_object_get_string(condition) << endl;
    cout << "Current Temperature      : " << json_object_get_string(temp) << temp_unit << endl;
    cout << "Feels Like               : " << json_object_get_string(feels_like) << temp_unit << endl;
    cout << "Min Temp                 : " << json_object_get_string(temp_min) << temp_unit << endl;
    cout << "Max Temp                 : " << json_object_get_string(temp_max) << temp_unit << endl;
    cout << "Pressure                 : " << json_object_get_string(pressure) << " hPa" << endl;
    cout << "Humidity                 : " << json_object_get_string(humidity) << "%" << endl;
    if (unit_sys == "Imperial")
        cout << "Visibility               : " << json_object_get_int(visibility) * 3.28084 << " ft" << endl;
    else
        cout << "Visibility               : " << json_object_get_string(visibility) << " m" << endl;
    cout << "Wind Speed               : " << json_object_get_string(speed) << speed_unit << endl;
    cout << "Wind Direction           : " << json_object_get_string(deg) << "°" << endl;
    cout << "\nPress 'Enter' Key to Return: ";
    getchar();
    splash();
}

void wb_parse_air_qual(void)
{
    char buffer[2048];
    FILE *fp;
    int air_qual;

    struct json_object *obj_parsed_json;
    struct json_object *obj_list;
    struct json_object *obj_list0;
    struct json_object *obj_main;
    struct json_object *obj_aqi;
    struct json_object *obj_components;
    struct json_object *obj_co;
    struct json_object *obj_no;
    struct json_object *obj_no2;
    struct json_object *obj_o3;
    struct json_object *obj_so2;
    struct json_object *obj_pm2_5;
    struct json_object *obj_pm10;
    struct json_object *obj_nh3;

    fp = fopen(DATA_FILE, "r");
    fread(buffer, 2048, 1, fp);
    fclose(fp);

    obj_parsed_json = json_tokener_parse(buffer);

    json_object_object_get_ex(obj_parsed_json, "list", &obj_list);
    if (json_object_get_type(obj_list) != json_type_array)
        return;
    obj_list0 = json_object_array_get_idx(obj_list, 0);
    json_object_object_get_ex(obj_list0, "main", &obj_main);
    json_object_object_get_ex(obj_main, "aqi", &obj_aqi);
    json_object_object_get_ex(obj_list0, "components", &obj_components);
    json_object_object_get_ex(obj_components, "co", &obj_co);
    json_object_object_get_ex(obj_components, "no", &obj_no);
    json_object_object_get_ex(obj_components, "no2", &obj_no2);
    json_object_object_get_ex(obj_components, "o3", &obj_o3);
    json_object_object_get_ex(obj_components, "so2", &obj_so2);
    json_object_object_get_ex(obj_components, "pm2_5", &obj_pm2_5);
    json_object_object_get_ex(obj_components, "pm10", &obj_pm10);
    json_object_object_get_ex(obj_components, "nh3", &obj_nh3);

    splash();

    air_qual = json_object_get_int(obj_aqi);
    switch (air_qual)
    {
    case 1:
        cout << "\033[0;37;42mAir Quality Index Score: 1 [Good]\033[0m\n\n";
        break;
    case 2:
        cout << "\033[0;37;46mAir Quality Index Score: 2 [Fair]\033[0m\n\n";
        break;
    case 3:
        cout << "\033[0;37;44mAir Quality Index Score: 3 [Moderate]\033[0m\n\n";
        break;
    case 4:
        cout << "\033[0;37;43mAir Quality Index Score: 4 [Poor]\033[0m\n\n";
        break;
    case 5:
        cout << "\033[0;37;41mAir Quality Index Score: 5 [Very Poor]\033[0m\n\n";
        break;
    default:
        break;
    }

    cout << "\033[0;30;47m"
         << "Air Component            : "
         << "Composition\033[0m" << endl;
    cout << "Carbon Monoxide [CO]     : " << json_object_get_string(obj_co) << " μg/m^3" << endl;
    cout << "Nitric Oxide [NO]        : " << json_object_get_string(obj_no) << " μg/m^3" << endl;
    cout << "Nitrogen Dioxide [NO2]   : " << json_object_get_string(obj_no2) << " μg/m^3" << endl;
    cout << "Ozone [O3]               : " << json_object_get_string(obj_o3) << " μg/m^3" << endl;
    cout << "Sulphur Dioxide [SO2]    : " << json_object_get_string(obj_so2) << " μg/m^3" << endl;
    cout << "Particulate Matter 2.5   : " << json_object_get_string(obj_pm2_5) << " μg/m^3" << endl;
    cout << "Particulate Matter 10    : " << json_object_get_string(obj_pm10) << " μg/m^3" << endl;
    cout << "Ammonia [NH3]            : " << json_object_get_string(obj_nh3) << " μg/m^3" << endl;
    cout << "\nPress 'Enter' Key to Return: ";
    getchar();
    splash();
}

void wb_parse_uvi(void)
{
    char buffer[2048];
    FILE *fp;
    string uvi_str;
    float uvi;

    struct json_object *obj_parsed_json;
    struct json_object *obj_current;
    struct json_object *obj_uvi;

    fp = fopen(DATA_FILE, "r");
    fread(buffer, 2048, 1, fp);
    fclose(fp);

    obj_parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(obj_parsed_json, "current", &obj_current);
    json_object_object_get_ex(obj_current, "uvi", &obj_uvi);

    uvi_str = json_object_get_string(obj_uvi);
    uvi = atof(uvi_str.c_str());

    splash();

    if (uvi < 2)
    {
        cout << "\033[0;37;42mUltraviolex Index: " << uvi << " [Low]\033[0m\n";
    }
    else if (uvi < 5)
    {
        cout << "\033[0;37;44mUltraviolex Index: " << uvi << " [Moderate]\033[0m\n";
    }
    else if (uvi < 7)
    {
        cout << "\033[0;37;43mUltraviolex Index: " << uvi << " [High]\033[0m\n";
    }
    else if (uvi < 10)
    {
        cout << "\033[0;37;41mUltraviolex Index: " << uvi << " [Very High]\033[0m\n";
    }
    else
    {
        cout << "\033[0;37;45mUltraviolex Index: " << uvi << " [Extreme]\033[0m\n";
    }

    cout << "\nPress 'Enter' Key to Return: ";
    getchar();
    splash();
}

void wb_parse_ambience(int mode)
{
    switch (mode)
    {
    case 1:
        wb_parse_weather();
        break;
    case 2:
        wb_parse_air_qual();
        break;
    case 3:
        wb_parse_uvi();
        break;
    default:
        break;
    }
}
