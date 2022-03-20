#include "Compromise.h"

using namespace std;

int All_count = 0;
bool Check = false;
Check_dealing Reset_do = zero_do;
Direction Reset_dir = Error;

int main(void)
{
    Game_Body Game_go;
    Game_go.Display();
    while (1)
    {
        if (!Check)
            Game_go.Deal_with_Input(Check, Reset_do, Reset_dir);
        All_count++;
        if (All_count == One_Second)
        {
            if (Reset_do == out_program)
                break;
            Game_go.Deal_with_Timer(Reset_do, Reset_dir);
            All_count = 0; // reset
            if (Check)
            {
                system("cls");
                Game_go.Display();
            }
            Check = false;
        }
    }
    return 0;
}