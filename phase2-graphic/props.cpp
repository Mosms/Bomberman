#include "props.h"

#include <health.h>
#include <move_control.h>
#include <bomb_control.h>

Props::Props(Item_check this_prop_type, QPointF this_pos, qreal Z_value, GameScene *belong_to):
    its_type(this_prop_type)
{
    this->prop = new GameObject();
    this->prop->addComponent(new ImageTransform(this_pos));
    this->Init_image();
    this->Set_zValue(Z_value);
    belong_to->attachGameObject(this->prop);
}
void Props::SetImage(const char *path)
{
    auto transform = this->prop->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    transform->setImage(path);
}
void Props::SetImage(const QString &path)
{
    auto transform = this->prop->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    transform->setImage(path);
}
void Props::Set_zValue(qreal z_Value)
{
    auto transform = this->prop->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    transform->setZValue(z_Value);
    transform->setAlignment(Qt::AlignBottom);
}//-------------（（（）））对齐在这里（（（）））------------//

void Props::Init_image()
{
    switch(this->its_type)
    {
    case Faster_speed:
        this->SetImage("../pic-use/props/faster-speed.png");
        break;
    case More_bomb:
        this->SetImage("../pic-use/props/more-bomb.png");
        break;
    case Stronger_bomb:
        this->SetImage("../pic-use/props/stronger-bomb.png");
        break;
    case Move_bomb:
        this->SetImage("../pic-use/props/move-bomb.png");
        break;
    case Healthier:
        this->SetImage("../pic-use/props/healthier.png");
        break;
    case Treasure:
        this->SetImage("../pic-use/props/treasure.png");
        break;
    case Nothing://Error
        Q_ASSERT(this->its_type == Faster_speed);
        break;
    }
}

void Props::Be_gotton(GameObject *Gainer)
{
    auto Move_change = Gainer->getComponent<Move_Control>();
    auto Bomb_change = Gainer->getComponent<Bomb_Control>();
    auto health = Gainer->getComponent<Health>();
    switch(this->its_type)
    {
    case Faster_speed:
        Move_change->Get_fast_item();
        break;
    case More_bomb:
        Bomb_change->Get_oneMore_bomb();
        break;
    case Stronger_bomb:
        Bomb_change->Make_bomb_stronger();
        break;
    case Move_bomb:
        Move_change->Enable_moving_bomb();
        break;
    case Healthier:
        health->One_more_blood();
        break;
    case Treasure:
        Move_change->Adding_score(rand() % Treasure_max);
        Move_change->Getting_a_Treasure();
        break;
    case Nothing://Error
        Q_ASSERT(this->its_type == Faster_speed);
        break;
    }//之后再把此对象删除即可。
    this->prop->destory(this->prop);
    delete this;
}
