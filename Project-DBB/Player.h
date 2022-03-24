#pragma once
#ifndef _PLAYER_ //标准写法：防止重复定义
#define _PLAYER_
/*0*/
#include <cmath>
#include <utility>
class iostream;
enum Direction
{
    Left,
    Right,
    Up,
    Down,
    Error = 100
};

/*Forward Declarations(前置声明)*/
/*1*/
class Player // class head
{
private: //只能在函数体内被“访问”，数据封装的思想，不想要被外界任意看到
    std::pair<int, int> Location;
    bool living;
    char symbol;
    int Score;       // display his score
    int Carry_num;   // useless in this stage
    bool Fast_speed; //
    int bomb_power;  // display how long it is
    bool Move_go;
    Direction move_dir;
    char It_should_be;

public:
    Player(std::pair<int, int> new_location, char Symbol) : Location(new_location),
                                                            symbol(Symbol),
                                                            Score(0),
                                                            living(true),
                                                            Carry_num(1),
                                                            Fast_speed(false),
                                                            bomb_power(1),
                                                            Move_go(false),
                                                            move_dir(Error),
                                                            It_should_be(' ') {}
    ~Player() { Location.first = Location.second = -1; /*no use at all*/ }

    /*void *operator new(size_t size)
    {
        void *p = malloc(size);
        memset(p, 0, size);
        return p;
    }
    void operator delete(void *p, size_t size);*/
    // 为了使用这个类的指针，就必须要重载new与delete
    //现学现卖，没有这两个会报错类定义不完全(貌似又不需要了)

    int Get_location_l() const { return Location.first; }
    int Get_location_c() const { return Location.second; }

    Direction Check_dir() const { return move_dir; }
    char Check_what_is_it() const { return symbol; }
    bool Check_living() const { return living; }
    int Check_speed() const { return Fast_speed; }
    int Check_bomb_power() const { return bomb_power; }
    int Check_score() const { return Score; }
    int Bomb_now_num() const { return Carry_num; }
    std::pair<int, int> Get_location() const { return Location; }
    char What_it_should_be() const { return It_should_be; }
    bool Check_if_move() const { return Move_go; }

    void Change_dir(Direction To_go)
    {
        move_dir = To_go;
        return;
    }
    void Sign_this_go()
    {
        Move_go = true;
        return;
    }
    void Change_there_sit(char this_is)
    {
        It_should_be = this_is;
        return;
    }
    void Reback_sit()
    {
        Move_go = false; // fix
        return;
    }
    void Change_pos(int First, int Second)
    {
        Location.first = First;
        Location.second = Second;
        return;
    }
    void Put_a_bomb()
    { // Assume that he has at least one
        Carry_num--;
        return;
    }
    void Back_a_bomb()
    {
        Carry_num++;
        return;
    }
    void Adding_score(int Value)
    {
        Score += Value;
        return;
    }
    //构造函数：必须存在这样的函数来创造对象

}; // class body (";"不可少)
/*Class Declarations(类-声明)*/
/*2*/
/*Class Definition(类-定义)*/
#endif