#include "game_control.h"

#include <imagetransform.h>
#include <gameobject.h>


control_game::control_game(Game_control this_type):
    Component(), this_control(this_type), control_map(nullptr),
    control_start(nullptr) {}
void control_game::Set_control_start(Start_game *this_one)
{
    this->control_start = this_one;
}
void control_game::Set_control_map(Gamemap *this_one)
{
    this->control_map = this_one;
}

void control_game::onClick(QGraphicsSceneMouseEvent *ev)
{
    switch(this->this_control)
    {
    case stop:
        this->gameObject->Stop_my_scene();
        this->Set_this_type(again);
        break;
    case again:
        this->gameObject->Rack_my_scene();
        this->Set_this_type(stop);
        break;
    case reack:
        Q_ASSERT(this->control_start != nullptr);
        this->control_start->Back_from_instru();
        break;
    case start:
        //this->control_what->Get_dettached(tmp_out);
        Q_ASSERT(this->control_start != nullptr);
        this->control_start->Real_start_();
        break;
    case instru:
        Q_ASSERT(this->control_start != nullptr);
        this->control_start->Instruction();
        break;
    case end:
        this->control_map->exit_game();
        break;
    case out:
        Q_ASSERT(this->control_start != nullptr);
        this->control_start->exit_game();
        break;
    default:
        Q_ASSERT(this->this_control == end);
        break;
    }
}
//void onAttach() ;
void control_game::onFirstUpdate()
{
    this->Set_this_type(this->this_control);
}//直接调用即可
void control_game::onUpdate(float deltaTime)
{
//
}
void control_game::Set_this_type(Game_control this_type)
{
    this->this_control = this_type;
    auto transform = this->gameObject->getComponent<ImageTransform>();
    transform->setAlignment(Qt::AlignBottom);
    transform->setZValue(prepare_fixed);
    Q_ASSERT(transform != nullptr);
    switch(this->this_control)
    {
    case stop:
        transform->setImage("../pic-use/others/stop.png");
        break;
    case again:
        transform->setImage("../pic-use/others/again.png");
        break;
    case reack:
        transform->setImage("../pic-use/others/reack.png");
        break;
    case start:
        transform->setImage("../pic-use/others/start1.png");
        break;
    case instru:
        transform->setImage("../pic-use/others/instru.png");
        break;
    case end:
        transform->setImage("../pic-use/others/over.png");
        break;
    case out:
        transform->setImage("../pic-use/others/exiting.png");
        break;
    default:
        Q_ASSERT(this->this_control == end);
        break;
    }
}
