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
#include "windows.h"
#include <utility>
#include <cassert>
#include <ctime>
#include <queue>

#ifndef _GAME__ //标准写法：防止重复定义
#define _GAME__
/*0*/
#define key_down(key_name) ((GetAsyncKeyState(key_name)) & 0x8000)
const int One_Second = 1000;    // We can change it if we really need
const int Movement_all_num = 3; // change this thing
const char init_map[Lines][Col] =
    {{'+', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '+'},
     {'|', ' ', ' ', '*', '*', '*', '#', '*', '#', '*', '#', '#', '*', '*', '*', ' ', ' ', '|'},
     {'|', ' ', '#', '#', '#', '*', '#', '*', '*', '*', '*', '*', '*', '#', '#', '#', ' ', '|'},
     {'|', '*', '#', '#', '*', '*', '#', '#', '#', '#', '*', '#', '#', '*', '#', '#', '*', '|'},
     {'|', '*', '*', '*', '*', '*', '#', '*', '*', '*', '*', '#', '#', '*', '*', '*', '*', '|'},
     {'|', '#', '#', '#', '#', '*', '#', '*', '#', '#', '#', '#', '#', '#', '#', '#', '*', '|'},
     {'|', '#', '*', '#', '*', '*', '#', '*', '*', '*', '*', '#', '*', '*', '*', '#', '*', '|'},
     {'|', '*', '*', '*', '*', '#', '#', '#', '#', '#', '*', '#', '#', '*', '#', '#', '*', '|'},
     {'|', '*', '#', '#', '*', '#', '*', '*', '*', '*', '*', '#', '*', '*', '#', '#', '*', '|'},
     {'|', '*', '#', '#', '*', '*', '*', '*', '#', '#', '#', '#', '#', '*', '*', '*', '*', '|'},
     {'|', '*', '*', '*', '*', '#', '#', '*', '*', '*', '*', '#', '*', '*', '#', '#', '*', '|'},
     {'|', '#', '*', '#', '*', '*', '#', '#', '#', '#', '*', '#', '*', '*', '*', '#', '*', '|'},
     {'|', '#', '#', '#', '#', '*', '#', '*', '*', '*', '*', '#', '#', '#', '#', '#', '*', '|'},
     {'|', '*', '*', '*', '*', '*', '#', '*', '#', '#', '#', '#', '#', '*', '*', '*', '*', '|'},
     {'|', '*', '#', '#', '*', '*', '#', '*', '*', '*', '*', '#', '#', '*', '#', '#', '*', '|'},
     {'|', ' ', '#', '#', '#', '*', '#', '#', '#', '#', '*', '*', '*', '#', '#', '#', ' ', '|'},
     {'|', ' ', ' ', '*', '*', '*', '#', '*', '*', '*', '*', '#', '*', '*', '*', ' ', ' ', '|'},
     {'+', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '+'}};
const char init_stuff[Lines][Col] = // 1代表加速度，2代表增加炸弹威力
    {{'+', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '+'},
     {'|', ' ', ' ', '2', '$', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', '$', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
     {'+', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '+'}}; // Used to init
                                                                                                  // Stuff only to check and search
const std::pair<int, int> people_1(1, 1);
const std::pair<int, int> people_2(1, 16); // change and fix
const std::pair<int, int> Computer_1(16, 1);
const std::pair<int, int> Computer_2(16, 16);

enum End
{
    normal_go = 666,
    Now_must_end,
    Normally_end
}; //更清晰明了

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
} // Maybe useful

class iostream;
class Player;
class Bomb;
class Gamemap;
class Game_Body;

/*Forward Declarations(前置声明)*/
/*1*/

class Game_Body // class head
{
private:
    Gamemap this_map;             //只能在函数体内被“访问”，数据封装的思想，不想要被外界任意看到
    Player human_1, human_2;      //经过三次修改，发现还是指针最好用()
    Player fake_1, fake_2;        //改过指针后发现自己把握不住
    int All_count, Timer;         // Timer 用来通过模运算来模拟定频率事件
    bool Check_every, Check_bomb; //只要有一个true Check_every 显示true就改变状态，刷新一次
    End Check_Ending;             //
    /* Direction *Reset_dir;*/    // history questions
    std::priority_queue<Bomb> Store_rest_bomb;

public:
    Game_Body() : this_map(init_map, init_stuff),
                  human_1(people_1, 'A'),
                  human_2(people_2, 'B'),
                  fake_1(Computer_1, 'C'),
                  fake_2(Computer_2, 'D'),
                  All_count(0),
                  Timer(0),
                  Check_Ending(normal_go),
                  Check_every(false),
                  Check_bomb(false)
    {
        Reset_movement();
        /*this_map.Delete_test();//only used to see my real map*/
    }
    ~Game_Body() {}
    bool Check_every_this() const { return Check_every; }
    int Check_All_count() const { return All_count; }
    bool Second_over() const { return All_count == One_Second; }
    bool Check_if_end() const { return Check_Ending != normal_go; }

    void Check_normally_ending()
    {
        if (Timer == 1e9)
            Check_Ending = Normally_end;
        int Dead_num = 0;
        Dead_num += (int)!human_1.Check_living();
        Dead_num += (int)!human_2.Check_living();
        Dead_num += (int)!fake_1.Check_living();
        Dead_num += (int)!fake_2.Check_living();
        if (Dead_num > 2)
            Check_Ending = Normally_end;
        return;
    }
    void Add_timer()
    {
        Timer++;
        return;
    }
    void Add_one_count()
    {
        All_count++;
        return;
    }
    void Display_a_barriar()
    {
        std::cout << "==================================" << std::endl;
        return;
    } // prepare_1
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
    } // prepare_2
    void This_Player_display(Player &thisone)
    {
        if (thisone.Check_living())
            if (this_map.Check_if_nothing(thisone.Get_location()))
                this_map.Change_real_map(thisone.Get_location(), thisone.Check_what_is_it());
            else
            {
                if (this_map.Check_if_is_thisone(thisone.Get_location(), 'O'))
                    thisone.Change_there_sit('O');
                this_map.Change_real_map(thisone.Get_location(), 'M');
            }
        return;
    } // prepare_3
    void Check_people_exist()
    {
        This_Player_display(human_1);
        This_Player_display(human_2);
        This_Player_display(fake_1);
        This_Player_display(fake_2);
        return;
    } // prepare_4
    void Reback_sit(Player &thisone)
    {
        this_map.Change_real_map(thisone.Get_location(), thisone.What_it_should_be());
        thisone.Change_there_sit(' '); // only used here to set ' '
        return;
    }
    void Reback_all_situation()
    {
        Reback_sit(human_1);
        Reback_sit(human_2);
        Reback_sit(fake_1);
        Reback_sit(fake_2);
        return;
    } // prepare_5
    void Display()
    {
        Display_a_barriar();
        std::cout << "     "
                  << "(Press Esc to terminate)" << std::endl;
        Display_a_barriar();
        std::cout << "积分规则：炸破软墙得10分" << std::endl
                  << "炸死其他玩家得100分" << std::endl
                  << "道具效果：道具一加快移动速度" << std::endl
                  << "道具二增加炸弹射程" << std::endl
                  << "Tips:地图$宝藏$等你发掘" << std::endl;
        Display_a_barriar();
        Check_people_exist();
        this_map.Print_now_situation();
        Reback_all_situation();
        if (Check_bomb)
        {
            this_map.Reverse_bombing();
            Check_bomb = false;
        }
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
        All_count = 0;
        Check_every = false;
        return;
    }
    void Move_that(Player &thisone)
    {
        int First = thisone.Get_location_l();
        int Second = thisone.Get_location_c();
        if (thisone.Check_dir() == Left && this_map.Check_go(First, Second - 1))
            Second--;
        if (thisone.Check_dir() == Right && this_map.Check_go(First, Second + 1))
            Second++;
        if (thisone.Check_dir() == Up && this_map.Check_go(First - 1, Second))
            First--; //要注意这里下标的真实含义
        if (thisone.Check_dir() == Down && this_map.Check_go(First + 1, Second))
            First++; // fake change over>>
        if (First != thisone.Get_location_l() || Second != thisone.Get_location_c())
        {
            Check_every = true;
            thisone.Change_pos(First, Second);
        }
        if (this_map.Check_if_is_thisone(First, Second, '1'))
            thisone.Fast_go(Timer + 10); // can change it
        if (this_map.Check_if_is_thisone(First, Second, '2'))
            thisone.Add_power(); // can change it
        if (this_map.Check_if_is_thisone(First, Second, '$'))
            thisone.Adding_score(80);
        return;
    }
    void Putting_bomb(Player *thisone)
    {
        if (thisone->Bomb_now_num() && this_map.Check_if_nothing(thisone->Get_location()))
        {
            thisone->Put_a_bomb();
            Bomb New_bomb(thisone, Timer);
            this_map.Change_real_map(New_bomb.Get_pos(), 'O');
            Store_rest_bomb.push(New_bomb);
        }
        return;
    }
    void Bombing(Bomb thisone)
    {
        int this_power = thisone.Get_power();
        std::pair<int, int> this_pos = thisone.Get_pos();
        Check_kill_people(this_pos, thisone);
        Check_bomb = true;
        for (int i = 1; i <= this_power && this_pos.first - i > 0; i++)
        {
            int First = this_pos.first - i, Second = this_pos.second;
            if (this_map.Check_if_nothing(First, Second))
            {
                Check_kill_people(First, Second, thisone);
                this_map.Change_real_map(First, Second, '|');
            }
            else if (this_map.Check_if_is_thisone(First, Second, '*'))
            {
                this_map.Change_real_to_stuff(First, Second);
                thisone.Check_master()->Adding_score(10);
                break;
            }
            else if (this_map.Check_if_is_thisone(First, Second, '#'))
                break;
            else if (this_map.Check_if_is_thisone(First, Second, 'O'))
                break;
            else
                continue; //例如碰到道具无变化
        }                 //上
        for (int i = 1; i <= this_power && this_pos.first + i < 17; i++)
        {
            int First = this_pos.first + i, Second = this_pos.second;
            if (this_map.Check_if_nothing(First, Second))
            {
                Check_kill_people(First, Second, thisone);
                this_map.Change_real_map(First, Second, '|');
            }
            else if (this_map.Check_if_is_thisone(First, Second, '*'))
            {
                this_map.Change_real_to_stuff(First, Second);
                thisone.Check_master()->Adding_score(10);
                break;
            }
            else if (this_map.Check_if_is_thisone(First, Second, '#'))
                break;
            else if (this_map.Check_if_is_thisone(First, Second, 'O'))
                break;
            else
                continue; //例如碰到道具无变化
        }                 //下
        for (int i = 1; i <= this_power && this_pos.second - i > 0; i++)
        {
            int First = this_pos.first, Second = this_pos.second - i;
            if (this_map.Check_if_nothing(First, Second))
            {
                Check_kill_people(First, Second, thisone);
                this_map.Change_real_map(First, Second, '-');
            }
            else if (this_map.Check_if_is_thisone(First, Second, '*'))
            {
                this_map.Change_real_to_stuff(First, Second);
                thisone.Check_master()->Adding_score(10);
                break;
            }
            else if (this_map.Check_if_is_thisone(First, Second, '#'))
                break;
            else if (this_map.Check_if_is_thisone(First, Second, 'O'))
                break;
            else
                continue; //例如碰到道具无变化
        }                 //左
        for (int i = 1; i <= this_power && this_pos.second + i < 17; i++)
        {
            int First = this_pos.first, Second = this_pos.second + i;
            if (this_map.Check_if_nothing(First, Second))
            {
                Check_kill_people(First, Second, thisone);
                this_map.Change_real_map(First, Second, '-');
            }
            else if (this_map.Check_if_is_thisone(First, Second, '*'))
            {
                this_map.Change_real_to_stuff(First, Second);
                thisone.Check_master()->Adding_score(10);
                break;
            }
            else if (this_map.Check_if_is_thisone(First, Second, '#'))
                break;
            else if (this_map.Check_if_is_thisone(First, Second, 'O'))
                break;
            else
                continue; //例如碰到道具无变化
        }                 //右
        return;
    }
    void Check_kill_people(std::pair<int, int> this_pos, Bomb thisone)
    {
        bool flag_check = false;
        if (thisone.Check_master()->Check_living() == true)
            flag_check = true;
        if (human_1.Get_location() == this_pos)
        {
            thisone.Check_master()->Adding_score(100);
            human_1.Dead_people();
        }
        if (human_2.Get_location() == this_pos)
        {
            thisone.Check_master()->Adding_score(100);
            human_2.Dead_people();
        }
        if (fake_1.Get_location() == this_pos)
        {
            thisone.Check_master()->Adding_score(100);
            fake_1.Dead_people();
        }
        if (fake_2.Get_location() == this_pos)
        {
            thisone.Check_master()->Adding_score(100);
            fake_2.Dead_people();
        }
        if (thisone.Check_master()->Check_living() == false && flag_check)
            thisone.Check_master()->Minus_score(100); //只用在最后检查一下即可
        return;
    }
    void Check_kill_people(int First, int Second, Bomb thisone)
    {
        std::pair<int, int> Change_pair(First, Second);
        Check_kill_people(Change_pair, thisone); //重载使用，应该没问题
        return;
    }
    void Timer_check_bomb()
    {
        if (!Store_rest_bomb.empty())
        {
            Bomb check_this = Store_rest_bomb.top();
            while (check_this.Check_if_to_bomb(Timer))
            {
                this_map.Change_real_map(check_this.Get_pos(), ' ');
                check_this.Check_master()->Back_a_bomb();
                Bombing(check_this);
                Store_rest_bomb.pop();
                if (Store_rest_bomb.empty())
                    return;
                else
                    check_this = Store_rest_bomb.top();
            }
        }
        return;
    }
    void Computer_timing_do()
    {
        if (Timer % 5 == 0)
        { //达到计时的效果
            if (Timer % 50 == 0)
            {
                if (fake_1.Check_living())
                    Putting_bomb(&fake_1);
                if (fake_2.Check_living())
                    Putting_bomb(&fake_2); //这时就可以体会到封装是真爽
            }
            srand((int)time(0));
            fake_1.Change_dir((Direction)(rand() % 4));
            fake_2.Change_dir((Direction)(rand() % 4));
            if (fake_1.Check_living())
                Move_that(fake_1);
            if (fake_2.Check_living())
                Move_that(fake_2);
            Check_every = true;
        }
        return;
    }
    void Human_timing_do()
    {
        if (human_1.Check_if_move() && human_1.Check_living())
        {
            Move_that(human_1);
            human_1.Reback_sit();
            Check_every = true;
        }
        if (human_2.Check_if_move() && human_2.Check_living())
        {
            Move_that(human_2);
            human_2.Reback_sit();
            Check_every = true;
        }
        return;
    }
    void Deal_with_Input()
    {
        if (_kbhit())
            _getch();
        if (key_down('a') || key_down('A'))
        {
            human_1.Sign_this_go();
            human_1.Change_dir(Left);
        }
        if (key_down('w') || key_down('W'))
        {
            human_1.Sign_this_go();
            human_1.Change_dir(Up);
        }
        if (key_down('s') || key_down('S'))
        {
            human_1.Sign_this_go();
            human_1.Change_dir(Down);
        }
        if (key_down('d') || key_down('D'))
        {
            human_1.Sign_this_go();
            human_1.Change_dir(Right);
        }
        if (key_down('j') || key_down('J'))
        {
            human_2.Sign_this_go();
            human_2.Change_dir(Left);
        }
        if (key_down('i') || key_down('I'))
        {
            human_2.Sign_this_go();
            human_2.Change_dir(Up);
        }
        if (key_down('k') || key_down('K'))
        {
            human_2.Sign_this_go();
            human_2.Change_dir(Down);
        }
        if (key_down('l') || key_down('L'))
        {
            human_2.Sign_this_go();
            human_2.Change_dir(Right);
        }
        if (key_down(0x1B)) // Esc----Search to find it
            Check_Ending = Now_must_end;
        if (key_down(0x20) && human_1.Check_living() && Timer > 5) // " "------Search to find it
            Putting_bomb(&human_1);                                //&&Timer>5防止一开始的误触
        if (key_down(0x0D) && human_2.Check_living() && Timer > 5) // Enter------Search to find it
            Putting_bomb(&human_2);
        return;
    }
    void Deal_with_Timer()
    {
        Timer_check_bomb();
        Computer_timing_do();
        Human_timing_do();
        Add_timer();
        return;
    }
}; // class body (";"不可少)
   /*Class Declarations(类-声明)*/
   /*2*/
   /*Class Definition(类-定义)*/

#endif