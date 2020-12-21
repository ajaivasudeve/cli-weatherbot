#include "weatherbot.h"

static std::string location_api_key;
std::string latitude;
std::string longitude;
std::string label;

int parse_location_data()
{
    char buffer[2048];
    FILE *fp;

    struct json_object *obj_parsed_json;
    struct json_object *obj_response;
    struct json_object *obj_view;
    struct json_object *obj_view0;
    struct json_object *obj_result;
    struct json_object *obj_result0;
    struct json_object *obj_location;
    struct json_object *obj_display_position;
    struct json_object *obj_latitude;
    struct json_object *obj_longitude;
    struct json_object *obj_address;
    struct json_object *obj_label;

    fp = fopen(DATA_FILE, "r");
    fread(buffer, 2048, 1, fp);
    fclose(fp);

    obj_parsed_json = json_tokener_parse(buffer);

    json_object_object_get_ex(obj_parsed_json, "Response", &obj_response);
    json_object_object_get_ex(obj_response, "View", &obj_view);
    if (json_object_get_type(obj_view) != json_type_array)
        return -1;
    obj_view0 = json_object_array_get_idx(obj_view, 0);
    json_object_object_get_ex(obj_view0, "Result", &obj_result);
    if (json_object_get_type(obj_result) != json_type_array)
        return -1;
    obj_result0 = json_object_array_get_idx(obj_result, 0);
    json_object_object_get_ex(obj_result0, "Location", &obj_location);
    json_object_object_get_ex(obj_location, "DisplayPosition", &obj_display_position);
    json_object_object_get_ex(obj_display_position, "Latitude", &obj_latitude);
    json_object_object_get_ex(obj_display_position, "Longitude", &obj_longitude);
    json_object_object_get_ex(obj_location, "Address", &obj_address);
    json_object_object_get_ex(obj_address, "Label", &obj_label);

    latitude = json_object_get_string(obj_latitude);
    longitude = json_object_get_string(obj_longitude);
    label = json_object_get_string(obj_label);

    return 0;
}

void load_location_api_key(void)
{
    ifstream myfile(LOCATION_API_KEY_FILE);
    if (myfile.is_open())
    {
        getline(myfile, location_api_key);
        myfile.close();
    }
}

void get_location_data()
{
    CURL *curl;
    FILE *fp;
    long result;
    string location, url;

    load_location_api_key();

    if (location_api_key == "0")
    {
        cout << "\033[0;37;41mError: Location API Key Not Found!\033[0m";
        cout << "\n\nPress 'Return' Key to Exit: ";
        getchar();
        return;
    }

    splash();
    cout << "\033[0;37;42m"
         << "Set Location: \n"
         << label << "  |  " << latitude << ", " << longitude << "\033[0m\n\n";
enter_name:
    cout << "\033[0;30;47mType 'EXIT' Return to Main Menu\033[0m\n";
    cout << "\nEnter Location Name: ";
    getline(cin, location);

    transform(location.begin(), location.end(), location.begin(), ::tolower);

    if (location == "exit")
    {
        splash();
        return;
    }

    location.erase(remove_if(location.begin(), location.end(), ::isspace), location.end());

    url = "https://geocoder.ls.hereapi.com/6.2/geocode.json?apiKey=" + location_api_key + "&searchtext=" + location;

    curl = curl_easy_init();
    fp = fopen(DATA_FILE, "wb");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    result = curl_easy_perform(curl);

    fclose(fp);
    curl_easy_cleanup(curl);

    splash();

    if (result == CURLE_OK)
    {
        result = parse_location_data();
        if (result)
            goto error;
        cout << "\033[0;37;42m"
             << "Set Location: \n"
             << label << "  |  " << latitude << ", " << longitude << "\033[0m\n\n";
        goto enter_name;
    }

error:
    cout << "\033[0;37;41mError: Location Invalid!\033[0m\n";
    goto enter_name;
}

void wb_auto_locate()
{
    CURL *curl;
    FILE *fp;
    long result;
    string url;
    char buffer[2048];

    struct json_object *obj_parsed_json;
    struct json_object *obj_place;
    struct json_object *obj_latitude;
    struct json_object *obj_longitude;

    url = "https://get.geojs.io/v1/ip/geo.json";

    curl = curl_easy_init();
    fp = fopen(DATA_FILE, "wb");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    result = curl_easy_perform(curl);

    fclose(fp);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
        return;

    fp = fopen(DATA_FILE, "r");
    fread(buffer, 2048, 1, fp);
    fclose(fp);

    obj_parsed_json = json_tokener_parse(buffer);

    json_object_object_get_ex(obj_parsed_json, "city", &obj_place);
    json_object_object_get_ex(obj_parsed_json, "latitude", &obj_latitude);
    json_object_object_get_ex(obj_parsed_json, "longitude", &obj_longitude);

    latitude = json_object_get_string(obj_latitude);
    longitude = json_object_get_string(obj_longitude);
    label = json_object_get_string(obj_place);
}
