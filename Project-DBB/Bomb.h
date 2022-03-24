#pragma once
#ifndef _BOMB_ //标准写法：防止重复定义
#define _BOMB_
/*0*/
#include <cmath>
#include "Player.h"

class iostream;
class Bomb;
/*Forward Declarations(前置声明)*/
/*1*/
class Bomb // class head
{
private: //只能在函数体内被“访问”，数据封装的思想，不想要被外界任意看到
    int power;
    int Bomb_ending_time;
    int Bombing_need_time;
    std::pair<int, int> Position;
    Player *Who_has_me;

public:
    Bomb(Player *thisone, int Starting_time) : power(thisone->Check_bomb_power()),
                                               Bomb_ending_time(Starting_time),
                                               Bombing_need_time(3), // to Change
                                               Position(thisone->Get_location()),
                                               Who_has_me(thisone)
    {
        Bomb_ending_time += Bombing_need_time;
    }
    ~Bomb() { power = 0; /*no use at all*/ }
    bool Check_if_to_bomb(int thistime) const { return Bomb_ending_time == thistime; }
    int Check_end_time() const { return Bomb_ending_time; }
    Player *Check_master() const { return Who_has_me; }

    int Get_power() const { return power; }
    std::pair<int, int> Get_pos() const { return Position; }
    bool operator<(const Bomb &thisone) const { return Bomb_ending_time > thisone.Check_end_time(); }
    //从小到大弹出，炸弹属于是剩的时间越少就优先级越高，越需要弹出
    //构造函数：必须存在这样的函数来创造对象

}; // class body (";"不可少)
/*Class Declarations(类-声明)*/
/*2*/
/*Class Definition(类-定义)*/
#endif