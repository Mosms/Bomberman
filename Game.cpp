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
#define key_down(key_name) ((GetAsyncKeyState(key_name)) & 0x8000)

const bool This_is_a_good_game = true;
//改变思路有些分离->完全使用Game类

int main(void)
{
    SetConsoleOutputCP(65001); //中文字体搞一波
    Game_Body Game_go;
    Game_go.Display();
    while (true)
    {
        Game_go.Deal_with_Input();
        Game_go.Add_one_count();
        if (Game_go.Second_over())
        {
            if (Game_go.Check_if_end())
                break;
            Game_go.Deal_with_Timer();
            if (This_is_a_good_game)
            {
                if (Game_go.Check_every_this())
                {
                    system("cls");
                    Game_go.Display();
                    Sleep(25);
                }
                if (Game_go.there_is_some_one_fast())
                {

                    Game_go.Fast_go();
                    system("cls");
                    Game_go.Display();
                }
                Sleep(25);
            }
            // two-25
            Game_go.Reset_movement(); // reset
            Game_go.Check_normally_ending();
            if (Game_go.Check_if_normally_end())
            {
                Game_go.Print_Winer();
                break;
            }
        }
    }
    return 0;
}