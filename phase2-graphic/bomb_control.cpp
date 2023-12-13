#include "bomb_control.h"

#include <move_control.h>
#include <bomb.h>
#include <iostream>

Bomb_Control::Bomb_Control(Control_type need_type, Gamemap *come_to):
    Component(), belong_to(come_to), how_control(need_type),
    Bomb_num(1), Adding_num(0), Bomb_strength(1), can_put(true) {}
void Bomb_Control::Put_Bomb()
{
    auto back_transform = this->gameObject->getComponent<ImageTransform>();
    auto this_place = this->belong_to->Its_map_place_in(this->gameObject->getComponent<ImageTransform>()->pos());
    auto Put_there_a_bomb = new GameObject();
    Put_there_a_bomb->addComponent(new ImageTransform(this_place));
    Put_there_a_bomb->getComponent<ImageTransform>()->setAlignment(Qt::AlignBottom);
    Put_there_a_bomb->getComponent<ImageTransform>()->setZValue(back_transform->zValue());
    auto bomb_attach = new Bomb_attach(this->Bomb_strength, Rain_bow, this->belong_to, this->gameObject, this_place);
    Put_there_a_bomb->addComponent(bomb_attach);
    this->belong_to->Get_attached(Put_there_a_bomb);
    this->belong_to->Get_bomb(Put_there_a_bomb);
    this->Bomb_num--;
    return;
}
void Bomb_Control::Back_a_bomb()
{
    this->Bomb_num++;
}
const int Bomb_Control::How_many_have()
{
    return this->Adding_num;
}
const int Bomb_Control::How_strong()
{
    return this->Bomb_strength - 1;
}
void Bomb_Control::onUpdate(float deltaTime)
{
    if(this->Bomb_num)
        switch(this->how_control)
        {
        case First_key_type:
            if(this->can_put)
                if(this->getKeyDown(Qt::Key_Space))
                {
                    this->Put_Bomb();
                    this->can_put = false;
                }
            break;
        case Second_key_type://STFW-->为了兼容大小键盘，只能如下这般设置
            if(this->can_put)
                if(this->getKeyDown(Qt::Key_Return) || this->getKeyDown(Qt::Key_Enter))
                {
                    this->Put_Bomb();
                    this->can_put = false;
                }
            break;
        case Computer_type:
            if(this->can_put)
                if(this->gameObject->getComponent<Move_Control>()->ExactBeen())
                    if(this->belong_to->put_bomb_or_not(this->gameObject))
                        this->Put_Bomb();
            break;
        default:
            assert(this->how_control == First_key_type);
            break;
        }
    switch(this->how_control)
    {
    case First_key_type:
        if(!this->can_put)
            if(this->getKeyUp(Qt::Key_Space))
                this->can_put = true;
        break;
    case Second_key_type://STFW-->为了兼容大小键盘，只能如下这般设置
        if(!this->can_put)
            if(this->getKeyUp(Qt::Key_Return) || this->getKeyUp(Qt::Key_Enter))
                this->can_put = true;
        break;
    case Computer_type:
        break;
    default:
        assert(this->how_control == First_key_type);
        break;
    }
}

void Bomb_Control::Get_oneMore_bomb()
{
    this->Bomb_num++;
    this->Adding_num++;
}
void Bomb_Control::Make_bomb_stronger()
{
    this->Bomb_strength++;
}
