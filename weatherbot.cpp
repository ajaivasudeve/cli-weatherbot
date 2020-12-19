#include "weatherbot.h"

void splash(void)
{
    system("clear");
    cout << "\n*--------------------------------------------*\n";
    cout << "|--------------------------------------------|\n";
    cout << "|--------------------------------------------|\n";
    cout << "|-----------------WeatherBot-----------------|\n";
    cout << "|--------------------------------------------|\n";
    cout << "|--------------------------------------------|\n";
    cout << "*--------------------------------------------*\n\n";
}

int main()
{
    int choice;

    wb_auto_locate();

    splash();

main_menu:
    cout << "\033[0;30;47mMain Menu:\033[0m"
            "\n\t1. Check Ambience"
            "\n\t2. Set Unit System"
            "\n\t3. Set Location"
            "\n\t4. Quit"
            "\n\nSelect an Option: ";
    cin >> choice;
    cin.clear();
    cin.ignore(10000, '\n');

    switch (choice)
    {
    case 1:
        wb_get_ambience_mode();
        goto main_menu;
        break;
    case 2:
        wb_set_unit_sys();
        goto main_menu;
        break;
    case 3:
        get_location_data();
        goto main_menu;
        break;
    case 4:
        break;
    default:
        splash();
        cout << "\033[0;37;41mError: Selection Invalid!\033[0m\n\n";
        goto main_menu;
        break;
    }

    return 0;
}
