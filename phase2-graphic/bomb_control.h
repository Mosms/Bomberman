#ifndef _BOMB_CONTROL_H_
#define _BOMB_CONTROL_H_

#include <physics.h>
#include <component.h>
#include <gameobject.h>
#include <imagetransform.h>
#include <gamemap.h>

class Bomb_Control : public Component
{
public:
    Bomb_Control(Control_type need_type, Gamemap *come_to);
private:
    void Put_Bomb();
public:
    void Back_a_bomb();
    const int How_many_have();
    const int How_strong();
public:
    void onUpdate(float deltaTime) override;
public:
    void Get_oneMore_bomb();
    void Make_bomb_stronger();

private:
    Gamemap *belong_to;
    Control_type how_control;
    int Bomb_num;
    int Adding_num;
    int Bomb_strength;
    bool can_put;
};//之后的想法是将炸弹控制与移动控制分开，这样可以减少各自的代码量，同时也更清晰。
#endif  // _BOMB_CONTROL_H_
