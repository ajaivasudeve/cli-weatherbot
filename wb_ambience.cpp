#include "weatherbot.h"

static std::string weather_api_key;
extern std::string unit_sys;
extern std::string latitude;
extern std::string longitude;
extern std::string label;

void load_weather_api_key(void)
{
    ifstream myfile(WEATHER_API_KEY_FILE);
    if (myfile.is_open())
    {
        getline(myfile, weather_api_key);
        myfile.close();
    }
}

void wb_get_ambience(int mode)
{
    CURL *curl;
    FILE *fp;
    string type, url;
    long ret;

    load_weather_api_key();

    if (weather_api_key == "0")
    {
        cout << "\033[0;37;41mError: API Key(s) Not Found!\033[0m";
        cout << "\n\nPress 'Enter' Key to Exit: ";
        getchar();
        return;
    }

    switch (mode)
    {
    case 1:
        type = "weather";
        break;
    case 2:
        type = "air_pollution";
        break;
    case 3:
        type = "onecall";
        break;
    default:
        break;
    }

    url = "http://api.openweathermap.org/data/2.5/" + type + "?lat=" + latitude + "&lon=" + longitude + "&appid=" + weather_api_key + "&units=" + unit_sys + "&exclude=minutely,hourly,daily,alerts";

    curl = curl_easy_init();
    fp = fopen(DATA_FILE, "w+");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    ret = curl_easy_perform(curl);

    fclose(fp);
    curl_easy_cleanup(curl);

    if (ret == CURLE_OK)
    {
        wb_parse_ambience(mode);
    }
    else
    {
        cout << "\033[0;37;41mError: Failed to fetch weather info!\033[0m";
    }
}

void wb_get_ambience_mode(void)
{
    int selection;

    splash();

ambience_menu:
    cout << "\033[0;30;47mSelect Data to Retrieve:\033[0m"
            "\n\t1. Weather Info"
            "\n\t2. Air Quality Info"
            "\n\t3. Ultraviolet Index"
            "\n\t4. Return to Main Menu"
            "\n\nSelect an Option: ";
    cin >> selection;
    cin.clear();
    cin.ignore(10000, '\n');

    switch (selection)
    {
    case 1:
    case 2:
    case 3:
        wb_get_ambience(selection);
        goto ambience_menu;
        break;
    case 4:
        splash();
        return;
        break;
    default:
        splash();
        cout << "\033[0;37;41mError: Selection Invalid!\033[0m\n\n";
        goto ambience_menu;
        break;
    }
}
