#include "weatherbot.h"

std::string unit_sys = "Metric";
std::string temp_unit = "° C";
std::string speed_unit = " m/s";

void wb_set_unit_sys(void)
{
    int selection;

    splash();

    cout << "\033[0;37;42m"
         << "Unit System Set - " << unit_sys << "\033[0m\n\n";

unit_menu:
    cout << "\033[0;30;47mSelect a Unit System:\033[0m"
            "\n\t1. Standard [Temp: Kelvin, Speed: Metre/Second, Dist: Metre]"
            "\n\t2. Metric   [Temp: Celcius, Speed: Metre/Second, Dist: Metre]"
            "\n\t3. Imperial [Temp: Fahrenheit, Speed: Miles/Hour, Dist: Feet]"
            "\n\t4. Return to Main Menu"
            "\n\nSelect an Option: ";
    cin >> selection;
    cin.clear();
    cin.ignore(10000, '\n');

    switch (selection)
    {
    case 1:
        unit_sys = "Standard";
        temp_unit = " K";
        speed_unit = " m/s";
        splash();
        cout << "\033[0;37;42m"
             << "Unit System Set - " << unit_sys << "\033[0m\n\n";
        goto unit_menu;
        break;
    case 2:
        unit_sys = "Metric";
        temp_unit = "° C";
        speed_unit = " m/s";
        splash();
        cout << "\033[0;37;42m"
             << "Unit System Set - " << unit_sys << "\033[0m\n\n";
        goto unit_menu;
        break;
    case 3:
        unit_sys = "Imperial";
        temp_unit = "° F";
        speed_unit = " MPH";
        splash();
        cout << "\033[0;37;42m"
             << "Unit System Set - " << unit_sys << "\033[0m\n\n";
        goto unit_menu;
        break;
    case 4:
        splash();
        break;
    default:
        splash();
        cout << "\033[0;37;41mError: Selection Invalid!\033[0m\n\n";
        goto unit_menu;
        break;
    }
}
