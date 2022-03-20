#pragma once
#ifndef _PLAYER_ //标准写法：防止重复定义
#define _PLAYER_
/*0*/
#include <cmath>
#include <utility>
class iostream;
class Player;
using namespace std;
/*Forward Declarations(前置声明)*/
/*1*/
class Player // class head
{
private: //只能在函数体内被“访问”，数据封装的思想，不想要被外界任意看到
    pair<int, int> Location;
    int Score; // display his score
    bool living;
    int Fast_speed;  // 0 is slow while othes represents the rest time
    int Strong_bomb; // a same situation superpower;

public:
    Player(pair<int, int> new_location) : Location(new_location), Score(0), living(true), Fast_speed(0), Strong_bomb(0) {}
    ~Player() { Location.first = Location.second = -1; /*no use at all*/ }
    pair<int, int> Get_location() { return Location; }
    bool Check_living() { return living; }
    int Check_speed() { return Fast_speed; }
    int Check_bomb() { return Strong_bomb; }
    int Check_score() { return Score; }
    void Change_pos(int First, int Second)
    {
        Location.first = First;
        Location.second = Second;
    }
    //构造函数：必须存在这样的函数来创造对象

}; // class body (";"不可少)
/*Class Declarations(类-声明)*/
/*2*/
/*Class Definition(类-定义)*/
#endif