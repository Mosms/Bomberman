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

public:
    Gamemap(const char instore[Lines][Col])
    {
        Map_itself = new char *[Lines];
        for (int i = 0; i < Lines; i++)
            Map_itself[i] = new char[Col];
        for (int i = 0; i < Lines; i++)
            for (int j = 0; j < Col; j++)
                Map_itself[i][j] = instore[i][j]; // copy
    }                                             //构造函数：必须存在这样的函数来创造对象
    ~Gamemap()
    {
        for (int i = 0; i < 16; i++)
            delete[] Map_itself[i];
        delete[] Map_itself;
    }
    void init_a_map(const char instore[Lines][Col])
    {
        for (int i = 0; i < Lines; i++)
            for (int j = 0; j < Col; j++)
                Map_itself[i][j] = instore[i][j];
    }
    char **Now_map() { return Map_itself; }
    void Print_now_situation()
    {
        for (int i = 0; i < Lines; i++)
            std::cout << "|       " << Map_itself[i] << "       |" << std::endl;
        return;
    }
}; // class body (";"不可少)
/*Class Declarations(类-声明)*/
/*2*/
/*Class Definition(类-定义)*/
#endif