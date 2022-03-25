#ifndef _MAPSQ_ //标准写法：防止重复定义
#define _MAPSQ_
/*0*/
#include <cmath>
#include <iostream>
//
class iostream;
class Gamemap;
/*Forward Declarations(前置声明)*/
const int Lines = 18, Col = 19; // change them
/*1*/
class Gamemap // class head
{
private: //只能在函数体内被“访问”，数据封装的思想，不想要被外界任意看到
    char **Map_itself;
    char **Stuff_mem;

public:
    Gamemap(const char instore[Lines][Col], const char Stuff[Lines][Col])
    {
        Map_itself = new char *[Lines];
        Stuff_mem = new char *[Lines];
        for (int i = 0; i < Lines; i++)
            Map_itself[i] = new char[Col];
        for (int i = 0; i < Lines; i++)
            for (int j = 0; j < Col; j++)
                Map_itself[i][j] = instore[i][j];
        for (int i = 0; i < Lines; i++)
            Stuff_mem[i] = new char[Col];
        for (int i = 0; i < Lines; i++)
            for (int j = 0; j < Col; j++)
                Stuff_mem[i][j] = Stuff[i][j]; // copy
    }                                          //构造函数：必须存在这样的函数来创造对象
    ~Gamemap()
    {
        for (int i = 0; i < 16; i++)
            delete[] Map_itself[i];
        delete[] Map_itself;
    }
    bool Check_if_nothing(std::pair<int, int> this_pos) const { return Map_itself[this_pos.first][this_pos.second] == ' '; }
    bool Check_if_nothing(int First, int Second) const { return Map_itself[First][Second] == ' '; }
    bool Check_if_is_thisone(std::pair<int, int> this_pos, char thisone) const { return Map_itself[this_pos.first][this_pos.second] == thisone; }
    bool Check_if_is_thisone(int First, int Second, char thisone) const { return Map_itself[First][Second] == thisone; }
    //
    bool Check_pick_stuff(std::pair<int, int> this_pos) const { return Map_itself[this_pos.first][this_pos.second] == '1' || Map_itself[this_pos.first][this_pos.second] == '2'; }
    bool Check_pick_stuff(int First, int Second) const { return Map_itself[First][Second] == '1' || Map_itself[First][Second] == '2'; }
    //
    bool Check_treasure(int First, int Second) const { return Map_itself[First][Second] == '$'; }
    bool Check_treasure(std::pair<int, int> this_pos) const { return Map_itself[this_pos.first][this_pos.second] == '$'; }
    //
    bool Check_go(int First, int Second) const
    {
        return Check_if_nothing(First, Second) || Check_pick_stuff(First, Second) || Check_treasure(First, Second);
    }
    bool Check_go(std::pair<int, int> this_pos) const
    {
        return Check_if_nothing(this_pos) || Check_pick_stuff(this_pos) || Check_treasure(this_pos);
    }
    //
    char What_is_stuff_here(int First, int Second) const { return Stuff_mem[First][Second]; }
    char What_is_really_here(int First, int Second) const { return Map_itself[First][Second]; }
    char What_is_stuff_here(std::pair<int, int> this_pos) const { return Stuff_mem[this_pos.first][this_pos.second]; }
    char What_is_really_here(std::pair<int, int> this_pos) const { return Map_itself[this_pos.first][this_pos.second]; }
    // Maybe useless but it doesn't influnce
    void Change_real_to_stuff(int First, int Second)
    {
        Map_itself[First][Second] = Stuff_mem[First][Second];
        return;
    }
    void Change_real_map(std::pair<int, int> thisone, char change_to)
    {
        Map_itself[thisone.first][thisone.second] = change_to;
        return;
    }
    void Change_real_map(int First, int Second, char change_to)
    {
        Map_itself[First][Second] = change_to;
        return;
    }
    void Change_stuff_map(std::pair<int, int> thisone, char change_to)
    {
        Stuff_mem[thisone.first][thisone.second] = change_to;
        return;
    }
    void Change_stuff_map(int First, int Second, char change_to)
    {
        Stuff_mem[First][Second] = change_to;
        return;
    }
    void init_a_map(const char instore[Lines][Col])
    {
        for (int i = 0; i < Lines; i++)
            for (int j = 0; j < Col; j++)
                Map_itself[i][j] = instore[i][j];
    }
    void Print_now_situation()
    {
        for (int i = 0; i < Lines; i++)
            std::cout << "|       " << Map_itself[i] << "       |" << std::endl;
        return;
    }
    void Delete_test()
    {
        for (int i = 1; i < Lines - 1; i++)
            for (int j = 1; j < Col - 2; j++)
                if (Map_itself[i][j] == '*')
                    Map_itself[i][j] = ' ';
        return;
    }
    void Reverse_bombing()
    {
        for (int i = 1; i < Lines - 1; i++)
            for (int j = 1; j < Col - 2; j++)
                if (Map_itself[i][j] == '-' || Map_itself[i][j] == '|')
                    Map_itself[i][j] = ' ';
        return;
    }
}; // class body (";"不可少)
/*Class Declarations(类-声明)*/
/*2*/
/*Class Definition(类-定义)*/
#endif