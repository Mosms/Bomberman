#define _HAS_STD_BYTE 0
#include <iostream>
#include <cstdlib>
#include "conio.h"
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "Bomb.h"
#include <climits>
#include "GameMap.h"
#include "Player.h"
#include "windows.h"
#include <utility>
#include <cassert>
#include <ctime>

#ifndef _GAME__ //标准写法：防止重复定义
#define _GAME__
/*0*/
#define key_down(key_name) ((GetAsyncKeyState(key_name)) ? 1 : 0)
const int One_Second = 1000;    // We can change it if we really need
const int Movement_all_num = 3; // change this thing
const char init_map[Lines][Col] =
    {{'+', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '+'},
     {'|', 'A', '#', '#', '#', 'B', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', 'D', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'+', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '+'}}; // Used to init
const std::pair<int, int> people_1(1, 1);
const std::pair<int, int> people_2(1, 5); // change and fix
const std::pair<int, int> Computer_1(9, 4);
const std::pair<int, int> Computer_2(16, 1);
enum Direction
{
    Left = 0,
    Right,
    Up,
    Down,
    Error = 100
};
enum Movement
{
    out_program = 0,
    People_1_move,
    People_2_move
}; //用来让数组更清晰简洁
Direction Change_dir(char get_kbit)
{
    if (get_kbit == 'a' || get_kbit == 'A' || get_kbit == 'j' || get_kbit == 'J')
        return Left;
    if (get_kbit == 'd' || get_kbit == 'D' || get_kbit == 'l' || get_kbit == 'L')
        return Right;
    if (get_kbit == 'w' || get_kbit == 'W' || get_kbit == 'i' || get_kbit == 'I')
        return Up;
    if (get_kbit == 's' || get_kbit == 'S' || get_kbit == 'k' || get_kbit == 'K')
        return Down;
    else
        return Error;
}

class iostream;
class Game_Body;
class Gamemap;
class Player;
/*Forward Declarations(前置声明)*/
/*1*/

class Game_Body // class head
{
private:
    Gamemap this_map; //只能在函数体内被“访问”，数据封装的思想，不想要被外界任意看到
    Player human_1, human_2;
    Player fake_1, fake_2;
    int All_count;
    bool Check_all, Check_every; //只要有一个true Check_every 显示true就改变状态，刷新一次
    bool *Reset_do;              // 0代表退出，第一个代表People1行动，第二个代表People2
    Direction *Reset_dir;

public:
    Game_Body() : this_map(init_map),
                  human_1(people_1),
                  human_2(people_2),
                  fake_1(Computer_1),
                  fake_2(Computer_2),
                  All_count(0),
                  Check_all(false),
                  Check_every(false)
    {
        Reset_do = new bool[Movement_all_num];
        Reset_dir = new Direction[Movement_all_num];
        Reset_movement();
    }
    ~Game_Body()
    {
        delete[] Reset_do;
        delete[] Reset_dir;
    }
    bool Check_all_this() { return Check_all; }
    bool Check_every_this() { return Check_every; }
    bool *Reset_do_this() { return Reset_do; }
    Direction *Reset_dir_this() { return Reset_dir; }
    int Check_All_count() { return All_count; }
    bool Check_if_over() { return Reset_do[out_program]; }
    bool Time_over() { return All_count == One_Second; }

    void Add_one_count()
    {
        All_count++;
        return;
    }
    void Display_a_barriar()
    {
        std::cout << "==================================" << std::endl;
        return;
    }
    void Display_score()
    {
        std::cout << "   "
                  << "Player1:" << human_1.Check_score() << std::endl;
        std::cout << "   "
                  << "Player2:" << human_2.Check_score() << std::endl;
        std::cout << "   "
                  << "Player3:" << fake_1.Check_score() << std::endl;
        std::cout << "   "
                  << "Player4:" << fake_2.Check_score() << std::endl;
        return;
    }
    void Display()
    {
        Display_a_barriar();
        std::cout << "    "
                  << "Press y (or Y) to terminate." << std::endl;
        Display_a_barriar();
        std::cout << "积分规则：炸破软墙得5分" << std::endl
                  << "炸死其他玩家得100分" << std::endl;
        Display_a_barriar();
        this_map.Print_now_situation();
        Display_a_barriar();
        std::cout << "        "
                  << "Score of Players:" << std::endl;
        Display_score();
        Display_a_barriar();
        std::cout << "                "
                  << "Nanjing University" << std::endl;
        Display_a_barriar();
        std::cout << "Made by fb, last changed:2022/3/21" << std::endl;
        Display_a_barriar();
        return;
    }
    void Reset_movement()
    {
        memset(Reset_do, false, Movement_all_num * sizeof(bool));
        memset(Reset_dir, Error, Movement_all_num * sizeof(Direction));
        All_count = 0;
        Check_all = Check_every = false;
        return;
    }
    void Check_all_finish()
    {
        if (Reset_do[out_program])
            Check_all = true;
        else
            for (int i = 1; i < Movement_all_num; i++)
                if (!Reset_do[i])
                {
                    Check_all = false;
                    return;
                }
        Check_all = true; // All_true then return true
        return;
    }
    void Check_if_any()
    {
        for (int i = 0; i < Movement_all_num; i++)
            if (Reset_do[i])
            {
                Check_every = true;
                return;
            }
        return;
    }
    void Move_that(Player &thisone, Direction dir)
    {
        int First = thisone.Get_location().first;
        int Second = thisone.Get_location().second;
        char check = this_map.Now_map()[thisone.Get_location().first][thisone.Get_location().second];
        if (dir == Left && this_map.Now_map()[First][Second - 1] == ' ')
            Second--;
        if (dir == Right && this_map.Now_map()[First][Second + 1] == ' ')
            Second++;
        if (dir == Up && this_map.Now_map()[First - 1][Second] == ' ')
            First--;
        if (dir == Down && this_map.Now_map()[First + 1][Second] == ' ')
            First++; // fake change over>>
        if (First != thisone.Get_location().first || Second != thisone.Get_location().second)
        {
            Check_every = true;
            this_map.Now_map()[First][Second] = check;
            this_map.Now_map()[thisone.Get_location().first][thisone.Get_location().second] = ' ';
        }
        thisone.Change_pos(First, Second);
    }
    void Deal_with_Input()
    {
        if (_kbhit())
        {
            _getch();
            if (key_down('a') || key_down('A'))
            {
                Reset_do[People_1_move] = true;
                Reset_dir[People_1_move] = Left;
            }
            if (key_down('w') || key_down('W'))
            {
                Reset_do[People_1_move] = true;
                Reset_dir[People_1_move] = Up;
            }
            if (key_down('s') || key_down('S'))
            {
                Reset_do[People_1_move] = true;
                Reset_dir[People_1_move] = Down;
            }
            if (key_down('d') || key_down('D'))
            {
                Reset_do[People_1_move] = true;
                Reset_dir[People_1_move] = Right;
            }
            if (key_down('j') || key_down('J'))
            {
                Reset_do[People_2_move] = true;
                Reset_dir[People_2_move] = Left;
            }
            if (key_down('i') || key_down('I'))
            {
                Reset_do[People_2_move] = true;
                Reset_dir[People_2_move] = Up;
            }
            if (key_down('k') || key_down('K'))
            {
                Reset_do[People_2_move] = true;
                Reset_dir[People_2_move] = Down;
            }
            if (key_down('l') || key_down('L'))
            {
                Reset_do[People_2_move] = true;
                Reset_dir[People_2_move] = Right;
            }
            if (key_down('Y') || key_down('y'))
                Reset_do[out_program] = true;
        }
        return;
    }
    void Deal_with_Timer()
    {
        srand((int)time(0));
        Move_that(fake_1, (Direction)(rand() % 4));
        Move_that(fake_2, (Direction)(rand() % 4));
        for (int i = 0; i < Movement_all_num; i++)
        {
            if (Reset_do[i])
                switch ((Movement)i)
                {
                case out_program:
                    break;
                case People_1_move:
                    Move_that(human_1, Reset_dir[People_1_move]);
                    break;
                case People_2_move:
                    Move_that(human_2, Reset_dir[People_2_move]);
                    break;
                default:
                    break;
                }
        }
        return;
    }
}; // class body (";"不可少)
   /*Class Declarations(类-声明)*/
   /*2*/
   /*Class Definition(类-定义)*/

#endif