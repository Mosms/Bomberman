#ifndef _MOVE_CONTROL_H_
#define _MOVE_CONTROL_H_

#include <constant_set.h>
#include <physics.h>
#include <component.h>
#include <gameobject.h>
#include <imagetransform.h>
#include <gamemap.h>

class Move_Control : public Component
{
public:
    Move_Control(Control_type need_type, Gamemap *come_to, Image_body_plan our_plan);

//----------------------同样下面这些私有类型都是只在此类内调用的-----------------------------//
private:
    void Init_set_pic(const char *path_h, const char *path_b, const char *path_l, const char *path_r);
private:
    void Set_velocity();
    bool Check_Move(float deltaTime);
private:
    void Move_this_human(float deltaTime);
    void Move_this_bot(float deltaTime);
    void Check_and_get_prop();
//----------------------这封装性..啧啧（真心不戳）-----------------------------//
public:
    //void onAttach() override;
    void onFirstUpdate() override;
    void onUpdate(float deltaTime) override;
public:
    void Get_fast_item();
    void Enable_moving_bomb();
    void Adding_score(int added);
    void Getting_a_Treasure();
    const int Get_score();
    const char *gethead_path();
    void set_score(GameObject *text_dis);
    const bool if_fast();
    const bool Can_MoveBomb();
    const bool ExactBeen();
    const int GetTreasurenum();
    Image_body_plan Skin_plan();

private:
    Image_body_plan skinHow;
    const char *head_path, *back_path, *left_path, *right_path;
    Gamemap *belong_to;
    GameObject *text_score;
    Control_type how_control;
    Dir Move_dir;
    QPointF velocity;
    float Move_bomb_t;
    float Fast_mode_t;
    int Score;
    int Treasure;
};//在定义中包含这么多的意图就是想将所有的控制交给Control来完成
//之后要做的就是挂载，框架应该（按理）来说应该会直接调用这些参数
#endif  // _MOVE_CONTROL_H_
//此类包含了所有需要控制人物的组件的行为具体包括有两套键盘控制以及一个 COMPUTER_CONTROL()
//原本想的是要定义分别的两个键盘控制与电脑控制分离，
//但是发现略为繁琐且没有必要，于是干脆打包带走，直接放到CONTROL里用枚举类具体来告诉我们是怎么控制的
