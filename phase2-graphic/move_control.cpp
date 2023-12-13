#include "move_control.h"

#include <iostream>

Move_Control::Move_Control(Control_type need_type, Gamemap *come_to, Image_body_plan our_plan):
    Component(), skinHow(our_plan), belong_to(come_to), how_control(need_type), Move_dir(center),
    velocity(Stop_velocity), Move_bomb_t(-1), Fast_mode_t(-1), Score(0), Treasure(0)
{
    switch(our_plan)
    {
    case _RED_:
        this->Init_set_pic("../pic-use/people/red/head.png", "../pic-use/people/red/back.png",
                           "../pic-use/people/red/left.png", "../pic-use/people/red/right.png");
        break;
    case _BLUE_:
        this->Init_set_pic("../pic-use/people/blue/head.png", "../pic-use/people/blue/back.png",
                           "../pic-use/people/blue/left.png", "../pic-use/people/blue/right.png");
        break;
    case _GREEN_:
        this->Init_set_pic("../pic-use/people/green/head.png", "../pic-use/people/green/back.png",
                           "../pic-use/people/green/left.png", "../pic-use/people/green/right.png");
        break;
    case _PINK_:
        this->Init_set_pic("../pic-use/people/pink/head.png", "../pic-use/people/pink/back.png",
                           "../pic-use/people/pink/left.png", "../pic-use/people/pink/right.png");
        break;
    case _YELLOW_:
        this->Init_set_pic("../pic-use/people/yellow/head.png", "../pic-use/people/yellow/back.png",
                           "../pic-use/people/yellow/left.png", "../pic-use/people/yellow/right.png");
        break;
    case _PURPLE_:
        this->Init_set_pic("../pic-use/people/purple/head.png", "../pic-use/people/purple/back.png",
                           "../pic-use/people/purple/left.png", "../pic-use/people/purple/right.png");
        break;
    default:
        Q_ASSERT(our_plan == _RED_);
        break;
    }
}
void Move_Control::Init_set_pic(const char *path_h, const char *path_b, const char *path_l, const char *path_r)
{
    this->head_path = path_h;
    this->back_path = path_b;
    this->left_path = path_l;
    this->right_path = path_r;
    return;
}
void Move_Control::Set_velocity()
{
    //int getrand = rand() % 20;
    auto transform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    auto judge_pos = this->gameObject->getComponent<ImageTransform>()->pos();
    QPoint now_place = this->belong_to->Its_map_place_ld(judge_pos);
    transform->setZValue(normal_fixed + (now_place.x() - 1));
    switch(this->how_control)
    {
    case First_key_type:
        if(this->getKey(Qt::Key_W))
            this->Move_dir = up;
        else if(this->getKey(Qt::Key_S))
            this->Move_dir = down;
        else if(this->getKey(Qt::Key_A))
            this->Move_dir = left;
        else if(this->getKey(Qt::Key_D))
            this->Move_dir = right;
        else this->Move_dir = center;
        break;
    case Second_key_type:
        if(this->getKey(Qt::Key_I))
            this->Move_dir = up;
        else if(this->getKey(Qt::Key_K))
            this->Move_dir = down;
        else if(this->getKey(Qt::Key_J))
            this->Move_dir = left;
        else if(this->getKey(Qt::Key_L))
            this->Move_dir = right;
        else this->Move_dir = center;
        break;
    case Computer_type://AL::PLAN
        if(Move_dir == center)//只有停下来的时候才设置方向，保证了每次只走一格
            this->Move_dir = this->belong_to->Give_computer_a_dir(this->gameObject);
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    switch(this->Move_dir)
    {
    case up:
        this->velocity = Up_velocity;//pre-set
        transform->setImage(this->back_path);
        break;
    case down:
        this->velocity = Down_velocity;
        transform->setImage(this->head_path);
        break;
    case left:
        this->velocity = Left_velocity;
        transform->setImage(this->left_path);
        break;
    case right:
        this->velocity = Right_velocity;
        transform->setImage(this->right_path);
        break;
    case center:
        this->velocity = Stop_velocity;
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}
bool Move_Control::Check_Move(float deltaTime)
{
    if(this->Move_bomb_t > 0)
        this->Move_bomb_t -= deltaTime;
    auto transform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    auto now_pos = transform->pos();
    //auto lt_place = this->belong_to->Its_map_place_lt(now_pos);
    //auto lt_pos = this->belong_to->Get_pos(lt_place);
    auto ld_place = this->belong_to->Its_map_place_ld(now_pos);
    auto ld_pos = this->belong_to->Get_pos(ld_place);
    qreal change_how_much = Init_velocity * (this->Fast_mode_t > 0 ? fast_muti : 1) * deltaTime;
    if(this->velocity == Stop_velocity) return true;
    else if(this->velocity.y() == 0)
    {
        if(now_pos.y() != ld_pos.y())
        {
            auto upper_pos = this->belong_to->Get_pos(ld_place.x() - 1, ld_place.y());
            if(now_pos.y() - upper_pos.y() < eplison_judge)
            {
                Q_ASSERT(now_pos.y() - upper_pos.y() >= 0);
                transform->setPos(upper_pos);
                return Check_Move(deltaTime);
            }
            else if(ld_pos.y() - now_pos.y() < eplison_judge)
            {
                Q_ASSERT(ld_pos.y() - now_pos.y() >= 0);
                transform->setPos(ld_pos);
                return Check_Move(deltaTime);
            }
            else
            {
                if(now_pos.y() - upper_pos.y() < slowly_move_judge)
                    now_pos.ry() -= Macro_move;
                if(ld_pos.y() - now_pos.y() < slowly_move_judge)
                    now_pos.ry() += Macro_move;
                transform->setPos(now_pos);//挤蹭并重设
                return false;
            }
        }
        else
        {
            qreal there_x = now_pos.x();
            qreal init_point = this->belong_to->Get_topleft().x();
            if(this->velocity == Left_velocity)
            {
                there_x -= change_how_much;
                int end_judge = std::floor((there_x - init_point) / pixblock);
                for(int i = ld_place.y() - 1; i >= end_judge; i--)
                    if(this->belong_to->Check_cannot_go(ld_place.x(), i))
                    {
                        if(this->Move_bomb_t > 0)
                            if(this->belong_to->Check_have_bomb(ld_place.x(), i))
                                this->belong_to->Set_bomb_dir(ld_place.x(), i, left);
                        transform->setPos(this->belong_to->Get_pos(ld_place.x(), i + 1));
                        return false;
                    }
                return true;
            }
            else  //Right_velocity
            {
                there_x += pixblock;//右移对于我们的对齐方式来说是比较特殊的
                there_x += change_how_much;
                int end_judge = std::floor((there_x - init_point) / pixblock);
                for(int i = ld_place.y() + 1; i <= end_judge; i++)
                    if(this->belong_to->Check_cannot_go(ld_place.x(), i))
                    {
                        if(this->Move_bomb_t > 0)
                            if(this->belong_to->Check_have_bomb(ld_place.x(), i))
                                this->belong_to->Set_bomb_dir(ld_place.x(), i, right);
                        transform->setPos(this->belong_to->Get_pos(ld_place.x(), i - 1));
                        return false;
                    }
                return true;
            }
        }
    }
    else if(this->velocity.x() == 0)
    {
        if(now_pos.x() != ld_pos.x())
        {
            auto right_pos = this->belong_to->Get_pos(ld_place.x(), ld_place.y() + 1);
            if(now_pos.x() - ld_pos.x() < eplison_judge)
            {
                Q_ASSERT(now_pos.x() - ld_pos.x() >= 0);
                transform->setPos(ld_pos);
                return Check_Move(deltaTime);
            }
            else if(right_pos.x() - now_pos.x() < eplison_judge)
            {
                Q_ASSERT(right_pos.x() - now_pos.x() >= 0);
                transform->setPos(right_pos);
                return Check_Move(deltaTime);
            }
            else
            {
                if(now_pos.x() - ld_pos.x() < slowly_move_judge)
                    now_pos.rx() -= Macro_move;
                if(right_pos.x() - now_pos.x() < slowly_move_judge)
                    now_pos.rx() += Macro_move;
                transform->setPos(now_pos);//挤蹭并重设
                return false;
            }
        }
        else
        {
            qreal there_y = now_pos.y();
            qreal init_point = this->belong_to->Get_topleft().y();
            if(this->velocity == Up_velocity)
            {
                there_y -= pixblock;
                there_y -= change_how_much;
                int end_judge = std::ceil((there_y - init_point) / pixblock);
                for(int i = ld_place.x() - 1; i >= end_judge; i--)
                    if(this->belong_to->Check_cannot_go(i, ld_place.y()))
                    {
                        if(this->Move_bomb_t > 0)
                            if(this->belong_to->Check_have_bomb(i, ld_place.y()))
                                this->belong_to->Set_bomb_dir(i, ld_place.y(), up);
                        transform->setPos(this->belong_to->Get_pos(i + 1, ld_place.y()));
                        return false;
                    }
                return true;
            }
            else  //Down_velocity
            {
                there_y += change_how_much;
                int end_judge = std::ceil((there_y - init_point) / pixblock);
                for(int i = ld_place.x() + 1; i <= end_judge; i++)
                    if(this->belong_to->Check_cannot_go(i, ld_place.y()))
                    {
                        if(this->Move_bomb_t > 0)
                            if(this->belong_to->Check_have_bomb(i, ld_place.y()))
                                this->belong_to->Set_bomb_dir(i, ld_place.y(), down);
                        transform->setPos(this->belong_to->Get_pos(i - 1, ld_place.y()));
                        return false;
                    }
                return true;
            }
        }
    }
    else
    {
        Q_ASSERT(this->velocity.isNull());
        return true;
    }
}//需要包含挤蹭的对齐操作
void Move_Control::Move_this_human(float deltaTime)
{
    //auto judge_pos = this->gameObject->getComponent<ImageTransform>()->pos();
    //QPoint now_place = this->belong_to->Its_map_place_ld(judge_pos);
    auto transform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    if(this->Check_Move(deltaTime))
    {
        if(this->Fast_mode_t > 0)
        {
            transform->setPos(transform->pos() + velocity * deltaTime * fast_muti);
            Fast_mode_t -= deltaTime;
        }//包含了快速移动的操作
        else
        {
            this->belong_to->Update_states();
            transform->setPos(transform->pos() + velocity * deltaTime);
        }
    }
}
void Move_Control::Move_this_bot(float deltaTime)
{
    //每次距离格点较近的时候都转化为center等待AI给予一个较好的方向
    if(this->Move_dir < center)
    {
        auto transform = this->gameObject->getComponent<ImageTransform>();
        Q_ASSERT(transform != nullptr);
        auto now_pos = transform->pos();
        //auto lt_place = this->belong_to->Its_map_place_lt(now_pos);
        //auto lt_pos = this->belong_to->Get_pos(lt_place);
        auto ld_place = this->belong_to->Its_map_place_ld(now_pos);
        auto ld_pos = this->belong_to->Get_pos(ld_place);
        auto next_pos = now_pos + velocity * deltaTime;
        if(Fast_mode_t > 0)
        {
            Fast_mode_t -= deltaTime;
            next_pos = now_pos + velocity * fast_muti * deltaTime;
        }
        if(this->Move_bomb_t > 0)
            this->Move_bomb_t -= deltaTime;
        auto next_place = this->belong_to->Its_map_place_ld(next_pos);
        Q_ASSERT(Move_dir < center);
        QPoint dir_judge = AllDir[Move_dir];
        if(ld_pos == now_pos) //刚刚开始走的时候判断即可
        {
            if(this->belong_to->Check_cannot_go(ld_place + dir_judge))
            {
                if(this->Move_bomb_t > 0)
                    if(this->belong_to->Check_have_bomb(ld_place + dir_judge))
                        this->belong_to->Set_bomb_dir(ld_place + dir_judge, Move_dir);
                Move_dir = center;
            }
            else transform->setPos(next_pos);
        }
        else if(ld_pos.x() == now_pos.x() || ld_pos.y() == now_pos.y())
        {
            if(next_place != ld_place)
            {
                if(Move_dir == left || Move_dir == down)
                    transform->setPos(ld_pos);
                else if(Move_dir == right || Move_dir == up)
                    transform->setPos(this->belong_to->Get_pos(next_place));
                else Q_ASSERT(Move_dir == center);
                Move_dir = center;
            }
            else//==//此时没有到终点，不需要改变方向
                transform->setPos(next_pos);
        }
        else Q_ASSERT(ld_pos.x() == now_pos.x() || ld_pos.y() == now_pos.y());//理应如此，因此直接assert保证
    }
    else return;//这时候有两种可能：分别是当前AI给出的策略就是停止不要移动以及已经无法移动了
}
void Move_Control::Check_and_get_prop()
{
    auto transform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    auto now_pos = transform->pos();
    auto lt_place = this->belong_to->Its_map_place_lt(now_pos);
    //auto lt_pos = this->belong_to->Get_pos(lt_place);
    auto ld_place = this->belong_to->Its_map_place_ld(now_pos);
    auto rd_place = this->belong_to->Its_map_place_rd(now_pos);
    auto ld_pos = this->belong_to->Get_pos(ld_place);
    if(ld_pos == now_pos)
        this->belong_to->Item_be_gotton(ld_place, this->gameObject);
    else if(ld_pos.x() == now_pos.x())
    {
        this->belong_to->Item_be_gotton(ld_place, this->gameObject);
        this->belong_to->Item_be_gotton(lt_place, this->gameObject);
    }
    else if(ld_pos.y() == now_pos.y())//更加完善且理所应当的道具获取
    {
        this->belong_to->Item_be_gotton(ld_place, this->gameObject);
        this->belong_to->Item_be_gotton(rd_place, this->gameObject);
    }
    else Q_ASSERT(ld_pos.y() == now_pos.y());//按控制角度来说这种情况不会出现
}
void Move_Control::onFirstUpdate()
{
    auto transform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    transform->setImage(this->head_path);//init
}//只有第一次时这样操作即可

void Move_Control::onUpdate(float deltaTime)
{
    this->Set_velocity();
    if(this->how_control == Computer_type)
        this->Move_this_bot(deltaTime);
    else if(this->how_control == First_key_type || this->how_control == Second_key_type)
        this->Move_this_human(deltaTime);
    else Q_ASSERT(this->how_control == Error_type);
    this->Check_and_get_prop();
    //this->Bomb();
    if(this->getKey(Qt::Key_1))
        this->belong_to->Check_map_good();
}
void Move_Control::Get_fast_item()
{
    if(this->Fast_mode_t > 0) this->Fast_mode_t += Fast_speed_time;
    else this->Fast_mode_t = Fast_speed_time;
}
void Move_Control::Enable_moving_bomb()
{
    if(this->Move_bomb_t > 0) this->Move_bomb_t += Moving_bomb_time;
    else this->Move_bomb_t = Moving_bomb_time;
}
void Move_Control::Adding_score(int added)
{
    this->Score += added;
}
void Move_Control::Getting_a_Treasure()
{
    this->Treasure++;
}
const int Move_Control::Get_score()
{
    return this->Score;
}
const char *Move_Control::gethead_path()
{
    return this->head_path;
}
void Move_Control::set_score(GameObject *text_dis)
{
    this->text_score = text_dis;
}
const bool Move_Control::if_fast()
{
    return this->Fast_mode_t > 0;
}
const bool Move_Control::Can_MoveBomb()
{
    return this->Move_bomb_t > 0;
}
const bool Move_Control::ExactBeen()
{
    return this->Move_dir == center;
}
const int Move_Control::GetTreasurenum()
{
    return this->Treasure;
}
Image_body_plan Move_Control::Skin_plan()
{
    return this->skinHow;
}
