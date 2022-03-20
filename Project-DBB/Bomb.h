#pragma once
#ifndef _BOMB_ //标准写法：防止重复定义
#define _BOMB_
/*0*/
#include <cmath>

class iostream;
class Bomb;
/*Forward Declarations(前置声明)*/
/*1*/
class Bomb // class head
{
private: //只能在函数体内被“访问”，数据封装的思想，不想要被外界任意看到
    int power;
    int Time_to_bomb;

public:
    Bomb(bool strong_force) : power(strong_force), Time_to_bomb(2) {}
    ~Bomb() { power = 0; /*no use at all*/ }
    void Check_time() { Time_to_bomb--; }
    bool Check_bomb() { return Time_to_bomb == 0; }
    int Get_power() { return power; }
    //构造函数：必须存在这样的函数来创造对象

}; // class body (";"不可少)
/*Class Declarations(类-声明)*/
/*2*/
/*Class Definition(类-定义)*/
#endif