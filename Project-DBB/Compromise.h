#pragma once
#include <iostream>
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

#ifndef _GAME__ //标准写法：防止重复定义
#define _GAME__
/*0*/

const int One_Second = 2e7; // We can change it if we really need
const char init_map[16][17] =
    {{'A', '#', '#', '#', 'B', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', 'D', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', '*', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
     {'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}}; // Used to init
const pair<int, int> people_1(0, 0);
const pair<int, int> people_2(0, 4); // change and fix
const pair<int, int> Computer_1(15, 0);
const pair<int, int> Computer_2(7, 0);
enum Direction
{
    Left = 100,
    Right,
    Up,
    Down,
    Error
};
enum Check_dealing
{
    out_program = -100,
    zero_do,
    p1_move,
    p2_move // fix it
};
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

public:
    Game_Body() : this_map(init_map), human_1(people_1), human_2(people_2), fake_1(Computer_1), fake_2(Computer_1) {}
    void Display()
    {
        this_map.Print_now_situation();
        // fix-here
        return;
    }
    void Move_that(Player &thisone, Direction dir)
    {
        int First = thisone.Get_location().first;
        int Second = thisone.Get_location().second;
        char check = this_map.Now_map()[thisone.Get_location().first][thisone.Get_location().second];
        if (dir == Left && Second > 0 && this_map.Now_map()[First][Second - 1] == ' ')
            Second--;
        if (dir == Right && Second < 15 && this_map.Now_map()[First][Second + 1] == ' ')
            Second++;
        if (dir == Up && First > 0 && this_map.Now_map()[First - 1][Second] == ' ')
            First--;
        if (dir == Down && First < 15 && this_map.Now_map()[First + 1][Second] == ' ')
            First++; // fake change over>>
        if (First != thisone.Get_location().first || Second != thisone.Get_location().second)
        {
            this_map.Now_map()[First][Second] = check;
            this_map.Now_map()[thisone.Get_location().first][thisone.Get_location().second] = ' ';
        }
        thisone.Change_pos(First, Second);
    }
    void Deal_with_Input(bool &check, Check_dealing &this_kbit, Direction &Dir)
    {
        char get_now;
        if (_kbhit())
            get_now = _getch();
        switch (get_now)
        {
        case 'a':
        case 'A':
        case 'w':
        case 'W':
        case 's':
        case 'S':
        case 'd':
        case 'D':
            Dir = Change_dir(get_now);
            this_kbit = p1_move;
            check = true;
            break;
        case 'i':
        case 'I':
        case 'j':
        case 'J':
        case 'k':
        case 'K':
        case 'l':
        case 'L':
            Dir = Change_dir(get_now);
            this_kbit = p2_move;
            check = true;
            break;
        // Fix that
        case '\\':
        case '|':
            this_kbit = out_program;
            check = true;
        default:
            break;
        }
        return;
    }
    void Deal_with_Timer(Check_dealing &this_kbit, Direction &Dir)
    {
        switch (this_kbit)
        {
        case p1_move:
            Move_that(human_1, Dir);
            break;
        case p2_move:
            Move_that(human_2, Dir);
            break;
        default:
            break;
        }
        this_kbit = zero_do;
        Dir = Error;
        return;
    }

}; // class body (";"不可少)
   /*Class Declarations(类-声明)*/
   /*2*/
   /*Class Definition(类-定义)*/

#endif