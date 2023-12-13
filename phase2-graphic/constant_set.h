#ifndef _CONSTANT_COE_H_
#define _CONSTANT_COE_H_
//this is a .h file especially set for storing our consant number

#include <component.h>
#include<QPointF>
//-----地图参数------//
const QPointF Excur(50, 100);
#define c_size 15
#define l_size 20
#define pixblock 40
#define winning 1800
//-----------------//

//-----道具演示------//
#define fontsize 22
#define dScores 0
const QPointF Exscores(30, -38);
#define dBloodNum 1
const QPointF ExBloodNum(148, -108);
#define dBombMore 2
const QPointF ExBombMore(210, -108);
#define dBombStronger 3
const QPointF ExBombStronger(148, -73);
#define dTreasure 4
const QPointF ExTreasure(210, -73);
#define dFaster 5
const QPointF ExFaster(148, -38);
#define dMovable 6
const QPointF ExMovable(210, -38);
#define dMax 6
const QPointF DisEx[7] = {Exscores, ExBloodNum, ExBombMore, ExBombStronger, ExTreasure, ExFaster, ExMovable};
const QPointF StateDisplayS(840, 84);
const QPointF AnotherPlus(0, 154);
//-----------------//

//-----移动策略参数------//
#define eplison_judge 5.0
#define slowly_move_judge 15.0
#define Macro_move 4.0
//-----快速道具效果参数------//
#define fast_muti 1.5
//-----------------//
//-----------------//

//-----初始血条------//
#define Init_blood 2
//-----------------//

//-----player_number------//
#define player_number 4
//-----------------//

//-----炸弹实现参数------//
#define skewing 0.5
#define Bombing_time 5.0
#define light_beam_time 1.5
#define Ready_to_bombing 0.3
#define Kill_people_adding_score 300
#define Bombing_people_add_score 50
#define Bombing_soft_wall_score 40
//-----------------//

//-----道具参数------//
#define Fast_speed_time 10.0
#define Moving_bomb_time 18.0
#define Treasure_max 150
//-----------------//

//-----人物移动速度参数------//
enum Dir
{
    left = 0,
    right,
    up,
    down,
    center
};//表示方向，甚至可以直接用到我们的数组里作index
const QPoint dir_left(0, -1);
const QPoint dir_right(0, 1);
const QPoint dir_up(-1, 0);
const QPoint dir_down(1, 0);
const QPoint AllDir[4] = {dir_left, dir_right, dir_up, dir_down};//由此直接用索引值即可
//这里的方向表示全部是由place表示的//
//------------------------//

//-------合理标记其zValue-------//
enum Z_level
{
    land_fixed = 400,
    normal_fixed,
    prepare_fixed = 666
};//now is enough but future my be not
//------------------------//

//-----人物移动速度参数------//
#define Init_velocity 150
const QPointF Left_velocity(-Init_velocity, 0);
const QPointF Right_velocity(Init_velocity, 0);
const QPointF Up_velocity(0, -Init_velocity);
const QPointF Down_velocity(0, Init_velocity);
const QPointF Stop_velocity(0, 0);
//----炸弹移动速度参数------//
#define b_mutilple 2
const QPointF bombLeft_velocity(-b_mutilple * Init_velocity, 0);
const QPointF bombRight_velocity(b_mutilple * Init_velocity, 0);
const QPointF bombUp_velocity(0, -b_mutilple * Init_velocity);
const QPointF bombDown_velocity(0, b_mutilple * Init_velocity);
//------------常数路径直接定义为常数即可，就不需要每次都操作了-----//
//不宏定义很容易出问题
#define center_1 "../pic-use/kill/center/1.png"
#define center_2 "../pic-use/kill/center/2.png"
#define center_3 "../pic-use/kill/center/3.png"
//center_path_set
#define up_b_full "../pic-use/kill/up/bound_a.png"
#define up_b_mid "../pic-use/kill/up/bound_m.png"
#define up_b_half "../pic-use/kill/up/bound_l.png"
#define up_c_path "../pic-use/kill/up/center.png"
//upper_path_set
#define down_b_full "../pic-use/kill/down/bound_a.png"
#define down_b_mid "../pic-use/kill/down/bound_m.png"
#define down_b_half "../pic-use/kill/down/bound_l.png"
#define down_c_path "../pic-use/kill/down/center.png"
//downside_path_set
#define left_b_full "../pic-use/kill/left/bound_a.png"
#define left_b_mid "../pic-use/kill/left/bound_m.png"
#define left_b_half "../pic-use/kill/left/bound_l.png"
#define left_c_path "../pic-use/kill/left/center.png"
//left_path_set
#define right_b_full "../pic-use/kill/right/bound_a.png"
#define right_b_mid "../pic-use/kill/right/bound_m.png"
#define right_b_half "../pic-use/kill/right/bound_l.png"
#define right_c_path "../pic-use/kill/right/center.png"
//right_path_set
//--------------------------------------------------------------------//
//-----人物初始位置参数------//
const QPoint Init_player_place_1(1, 1);
const QPoint Init_player_place_2(1, 18);
const QPoint Init_player_place_3(13, 1);
const QPoint Init_player_place_4(13, 18);
//------------------------------------//

enum Control_type
{
    First_key_type = 1000, //表示第一套控制方案WASD
    Second_key_type, //表示第二套控制方案JIKL
    Computer_type,//电脑控制类
    Error_type
};//控制类型枚举
//------------------------------------//
enum Image_body_plan//皮肤枚举
{
    _RED_ = 23,
    _BLUE_,
    _GREEN_,
    _PINK_,
    _YELLOW_,
    _PURPLE_,
    _Error_
};//if later we need to complete the work of choice of player_color, we can use this enum
enum bomb_image//炸弹皮肤枚举
{
    Blue_one = 2000,
    Rain_bow,
    blue_red_cmp,
    Red_one,
    Error//we wish not to use that()
};
//----------关于皮肤的枚举类--------------//
enum Wall_check//墙类型枚举
{
    _S_ = 200,//soft_wall
    _H_,//hard_wall
    _N_,//Nothing
    _E_//Check_error
};//最好保证大小一致，这样可以让下面的更改更方便一点
enum Item_check//道具类型枚举
{
    Faster_speed = 1200,//FAST_SPEED
    More_bomb,//MORE_BOMB
    Stronger_bomb,//STRONGER_BOMB
    Move_bomb,//CAN_MOVE_BOMB
    Healthier,//更多血条
    Treasure,
    Nothing//nothing//Error
};
//------------------------------------//
#define interval_gap 15
enum Bombing_state//炸弹显示的先行状态，用来实现动态效果
{
    normal = 9090,
    change,
    over
};
enum Kill_state//同理，用于实现光波的动态效果
{
    full_s = 787,
    mid_s,
    half_s,
    mid_later
};
//-----关于控制炸弹的动态效果的枚举类-------//
//-----控制游戏的参数------//
enum Game_control
{
    stop = 73823,//游戏暂停
    again,//暂停返回
    reack,//回到游戏
    start,//最初的游戏开始
    instru,//说明界面
    end,//游戏终止
    out//开始界面的退出游戏
};
#define yes_have "Y"
#define not_have "N"
const QPointF ButtonStop(960, 640);
const QPointF ButtonEnd(1040, 640);
const QPointF play_center(200, 200);
//-----------------//
//----------------------------------------------这里仍然使用了固定数组的形式来标记初始地图----------------------------------------//
const Wall_check Check_Wall[15][20] =
{
    {_H_, _H_, _H_, _H_, _H_,/*1*/ _H_, _H_, _H_, _H_, _H_,/*2*/ _H_, _H_, _H_, _H_, _H_,/*3*/ _H_, _H_, _H_, _H_, _H_},
    {_H_, _N_, _N_, _S_, _H_,/*1*/ _H_, _N_, _S_, _H_, _N_,/*2*/ _S_, _H_, _S_, _N_, _H_,/*3*/ _H_, _S_, _N_, _N_, _H_},
    {_H_, _N_, _H_, _S_, _N_,/*1*/ _H_, _H_, _N_, _H_, _S_,/*2*/ _H_, _H_, _N_, _H_, _H_,/*3*/ _N_, _S_, _H_, _N_, _H_},
    {_H_, _S_, _H_, _H_, _S_,/*1*/ _N_, _S_, _S_, _N_, _S_,/*2*/ _S_, _N_, _S_, _S_, _N_,/*3*/ _S_, _H_, _H_, _S_, _H_},
    {_H_, _S_, _N_, _S_, _S_,/*1*/ _H_, _H_, _H_, _H_, _H_,/*2*/ _S_, _H_, _H_, _H_, _H_,/*3*/ _S_, _S_, _N_, _S_, _H_},
    {_H_, _H_, _S_, _H_, _H_,/*1*/ _H_, _S_, _S_, _N_, _S_,/*2*/ _S_, _N_, _S_, _S_, _H_,/*3*/ _H_, _H_, _S_, _H_, _H_},
    {_H_, _S_, _N_, _S_, _N_,/*1*/ _H_, _N_, _H_, _H_, _H_,/*2*/ _H_, _H_, _H_, _N_, _H_,/*3*/ _N_, _S_, _N_, _S_, _H_},
    {_H_, _H_, _H_, _H_, _S_,/*1*/ _S_, _S_, _H_, _H_, _H_,/*2*/ _H_, _H_, _H_, _S_, _S_,/*3*/ _S_, _H_, _H_, _H_, _H_},
    {_H_, _S_, _N_, _S_, _N_,/*1*/ _H_, _N_, _H_, _H_, _H_,/*2*/ _H_, _H_, _H_, _N_, _H_,/*3*/ _N_, _S_, _N_, _S_, _H_},
    {_H_, _H_, _S_, _H_, _H_,/*1*/ _H_, _S_, _S_, _N_, _S_,/*2*/ _S_, _N_, _S_, _S_, _H_,/*3*/ _H_, _H_, _S_, _H_, _H_},
    {_H_, _S_, _N_, _S_, _S_,/*1*/ _H_, _H_, _H_, _H_, _N_,/*2*/ _H_, _H_, _H_, _H_, _H_,/*3*/ _S_, _S_, _N_, _S_, _H_},
    {_H_, _S_, _H_, _H_, _S_,/*1*/ _N_, _S_, _S_, _N_, _S_,/*2*/ _S_, _N_, _S_, _S_, _N_,/*3*/ _S_, _H_, _H_, _S_, _H_},
    {_H_, _N_, _H_, _S_, _N_,/*1*/ _H_, _H_, _N_, _H_, _H_,/*2*/ _N_, _H_, _N_, _H_, _H_,/*3*/ _N_, _S_, _H_, _N_, _H_},
    {_H_, _N_, _N_, _S_, _H_,/*1*/ _H_, _N_, _S_, _H_, _N_,/*2*/ _S_, _H_, _S_, _N_, _H_,/*3*/ _H_, _S_, _N_, _N_, _H_},
    {_H_, _H_, _H_, _H_, _H_,/*1*/ _H_, _H_, _H_, _H_, _H_,/*2*/ _H_, _H_, _H_, _H_, _H_,/*3*/ _H_, _H_, _H_, _H_, _H_}
};
//----------------------------------------------这里仍然使用了固定数组的形式来标记初始地图----------------------------------------//
//================AI寻路算法所需================//
struct way_memory
{
    int priority;
    int NowCost;
};

#endif
