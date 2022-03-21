#define _HAS_STD_BYTE 0
#include "conio.h"
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "Bomb.h"
#include <climits>
#include "GameMap.h"
#include "Player.h"
#include <utility>
#include <cassert>
#include "windows.h"
#include "Compromise.h"
#define key_down(key_name) ((GetAsyncKeyState(key_name)) ? 1 : 0)
//改变思路有些分离->完全使用Game类

int main(void)
{
    SetConsoleOutputCP(65001);
    Game_Body Game_go;
    Game_go.Display();
    while (true)
    {
        if (!Game_go.Check_all_this()) // use to balance
        {
            Game_go.Deal_with_Input();
            Game_go.Check_all_finish();
        }
        Game_go.Add_one_count();
        if (Game_go.Time_over())
        {
            if (Game_go.Check_if_over())
                break;
            Game_go.Check_if_any();
            Game_go.Deal_with_Timer();
            if (Game_go.Check_every_this())
            {
                system("cls");
                Game_go.Display();
                Sleep(80);
            }
            Game_go.Reset_movement(); // reset
        }
    }
    return 0;
}