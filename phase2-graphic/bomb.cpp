#include "bomb.h"

#include <bomb_control.h>
#include <move_control.h>
#include <iostream>

Bomb_attach::Bomb_attach(int this_strength, bomb_image set_one, Gamemap *this_map, GameObject *come_from, QPoint start_bomb):
    Bomb_strength(this_strength), master(come_from), belong_to(this_map),
    Start(start_bomb), Now_bomb_state(normal), Now_kill_state(full_s), bombing(false),
    interval(0), Bombing_left_t(Bombing_time), Kill_left_t(light_beam_time), Moving(center)
{
    switch(set_one)
    {
    case Blue_one:
        this->bombing_path_normal = "../pic-use/bomb/blue/normal.png";
        this->bombing_path_change = "../pic-use/bomb/blue/change.png";
        this->bombing_path_over = "../pic-use/bomb/blue/over.png";
        break;
    case Red_one:
        this->bombing_path_normal = "../pic-use/bomb/red/normal.png";
        this->bombing_path_change = "../pic-use/bomb/red/change.png";
        this->bombing_path_over = "../pic-use/bomb/red/over.png";
        break;
    case Rain_bow:
        this->bombing_path_normal = "../pic-use/bomb/bow/normal.png";
        this->bombing_path_change = "../pic-use/bomb/bow/change.png";
        this->bombing_path_over = "../pic-use/bomb/bow/over.png";
        break;
    case blue_red_cmp:
        this->bombing_path_normal = "../pic-use/bomb/comp/normal.png";
        this->bombing_path_change = "../pic-use/bomb/comp/change.png";
        this->bombing_path_over = "../pic-use/bomb/comp/over.png";
        break;
    default:
        Q_ASSERT(set_one == Error);
        break;
    }//这个图片是变换的，或者说可更改的，与之形成对比的爆炸光束是固定的设置
    for(int i = 0; i < 5; i++)
        this->Edge_and_center[i] = -1;
    for(int i = 0; i < player_number; i++)
        this->this_kill[i] = false;
}
const char *Bomb_attach::get_image_center(Dir this_dir)
{
    switch(this_dir)
    {
    case left:
        return left_c_path;
        break;
    case right:
        return right_c_path;
        break;
    case up:
        return up_c_path;
        break;
    case down:
        return down_c_path;
        break;
    case center:
        Q_ASSERT(this_dir == left);
        return center_1;
        break;//虽然知道不会用到，还是要象征性设置一下
    default:
        Q_ASSERT(this_dir == left);
        return "False";
        break;
    }
    return "False";
}
const char *Bomb_attach::get_image_bound_full(Dir this_dir)
{
    switch(this_dir)
    {
    case left:
        return left_b_full;
        break;
    case right:
        return right_b_full;
        break;
    case up:
        return up_b_full;
        break;
    case down:
        return down_b_full;
        break;
    case center:
        return center_1;
        break;
    default:
        Q_ASSERT(this_dir == left);
        return "False";
        break;
    }
    return "False";
}
const char *Bomb_attach::get_image_bound_mid(Dir this_dir)
{
    switch(this_dir)
    {
    case left:
        return left_b_mid;
        break;
    case right:
        return right_b_mid;
        break;
    case up:
        return up_b_mid;
        break;
    case down:
        return down_b_mid;
        break;
    case center:
        return center_2;
        break;
    default:
        Q_ASSERT(this_dir == left);
        return "False";
        break;
    }
    return "False";
}
const char *Bomb_attach::get_image_bound_half(Dir this_dir)
{
    switch(this_dir)
    {
    case left:
        return left_b_half;
        break;
    case right:
        return right_b_half;
        break;
    case up:
        return up_b_half;
        break;
    case down:
        return down_b_half;
        break;
    case center:
    default:
        Q_ASSERT(this_dir == left);
        return "False";
        break;
    }
    return "False";
}

void Bomb_attach::Set_dir_move(Dir set_this)
{
    this->Moving = set_this;
}

void Bomb_attach::Change_wave_image()
{
    Q_ASSERT(!this->kill_wave.empty());
    //std::cout << "I'm here!!!" << std::endl;
    for(int i = 0; i < 5; i++)
    {
        if(this->Edge_and_center[i] < 0)
            continue;
        else switch(this->Now_kill_state)
            {
            case full_s:
                this->kill_wave[this->Edge_and_center[i]]->getComponent<ImageTransform>()
                ->setImage(this->get_image_bound_mid(Dir(i)));
                break;
            case mid_s:
                this->kill_wave[this->Edge_and_center[i]]->getComponent<ImageTransform>()
                ->setImage(this->get_image_bound_full(Dir(i)));
                break;
            case half_s:
                this->kill_wave[this->Edge_and_center[i]]->getComponent<ImageTransform>()
                ->setImage(this->get_image_bound_mid(Dir(i)));
                break;
            case mid_later:
                this->kill_wave[this->Edge_and_center[i]]->getComponent<ImageTransform>()
                ->setImage(this->get_image_bound_full(Dir(i)));
                break;
            default:
                break;
            }
    }
    switch(this->Now_kill_state)
    {
    case full_s:
        this->Now_kill_state = mid_s;
        break;
    case mid_s:
        this->Now_kill_state = half_s;
        break;
    case half_s:
        this->Now_kill_state = mid_later;
        break;
    case mid_later:
        this->Now_kill_state = full_s;
        break;
    default:
        break;
    }//循环过后统一换，我们主要是要保证其变化一致性

}
void Bomb_attach::Check_and_move(float deltaTime)
{
    auto transform = this->gameObject->getComponent<ImageTransform>();
    if(this->Moving != center)
    {
        auto this_pos = transform->pos();
        auto ld = this->belong_to->Its_map_place_ld(this_pos);
        auto lt = this->belong_to->Its_map_place_lt(this_pos);
        auto rd = this->belong_to->Its_map_place_rd(this_pos);
        auto ld_pos = this->belong_to->Get_pos(ld);
        switch(this->Moving)
        {
        case left:
            Q_ASSERT(this_pos.y() == ld_pos.y());
            if(this_pos.x() == ld_pos.x())
            {
                if(this->belong_to->Check_have_Wall(ld.x(), ld.y() - 1))
                {
                    this->Start = ld;
                    this->Moving = center;
                }
                else transform->setPos(this_pos + bombLeft_velocity * deltaTime);
            }
            else
            {
                if(this->belong_to->Check_have_Wall
                        (this->belong_to->Its_map_place_ld(this_pos + bombLeft_velocity * deltaTime)))
                {
                    this->Start = ld;
                    transform->setPos(ld_pos);
                    this->Moving = center;
                }
                else transform->setPos(this_pos + bombLeft_velocity * deltaTime);
            }
            break;
        case right:
            Q_ASSERT(this_pos.y() == ld_pos.y());
            if(this_pos.x() == ld_pos.x())
            {
                if(this->belong_to->Check_have_Wall(ld.x(), ld.y() + 1))
                {
                    this->Start = ld;
                    this->Moving = center;
                }
                else transform->setPos(this_pos + bombRight_velocity * deltaTime);
            }
            else
            {
                auto judge_pos = this_pos + bombRight_velocity * deltaTime;
                judge_pos.rx() += 40;
                if(this->belong_to->Check_have_Wall
                        (this->belong_to->Its_map_place_ld(judge_pos)))
                {
                    this->Start = rd;
                    transform->setPos(this->belong_to->Get_pos(rd));
                    this->Moving = center;
                }
                else transform->setPos(this_pos + bombRight_velocity * deltaTime);
            }
            break;
        case up:
            Q_ASSERT(this_pos.x() == ld_pos.x());
            if(this_pos.y() == ld_pos.y())
            {
                if(this->belong_to->Check_have_Wall(ld.x() - 1, ld.y()))
                {
                    this->Start = ld;
                    this->Moving = center;
                }
                else transform->setPos(this_pos + bombUp_velocity * deltaTime);
            }
            else
            {
                auto judge_pos = this_pos + bombUp_velocity * deltaTime;
                judge_pos.ry() -= 40;
                if(this->belong_to->Check_have_Wall
                        (this->belong_to->Its_map_place_ld(judge_pos)))
                {
                    this->Start = lt;
                    transform->setPos(this->belong_to->Get_pos(lt));
                    this->Moving = center;
                }
                else transform->setPos(this_pos + bombUp_velocity * deltaTime);
            }
            break;
        case down:
            Q_ASSERT(this_pos.x() == ld_pos.x());
            if(this_pos.y() == ld_pos.y())
            {
                if(this->belong_to->Check_have_Wall(ld.x() + 1, ld.y()))
                {
                    this->Start = ld;
                    this->Moving = center;
                }

                else transform->setPos(this_pos + bombDown_velocity * deltaTime);
            }
            else
            {
                if(this->belong_to->Check_have_Wall
                        (this->belong_to->Its_map_place_ld(this_pos + bombDown_velocity * deltaTime)))
                {
                    this->Start = ld;
                    transform->setPos(ld_pos);
                    this->Moving = center;
                }
                else transform->setPos(this_pos + bombDown_velocity * deltaTime);
            }
            break;
        default:
            Q_ASSERT(this->Moving == center);
            break;
        }
    }
    this->Start = this->belong_to->Its_map_place_ld(transform->pos());//upgrade
    transform->setZValue(this->belong_to->Z_this_value(this->Start));
}
void Bomb_attach::Bombing_and_over(float deltaTime)
{
    //在这里要注意炸弹爆炸时的位置要重新确定
    assert(!(this->Bombing_left_t > 0));
    this->Kill_left_t -= deltaTime;
    if(this->bombing)//表示已经炸了
    {
        if(this->Kill_left_t > 0)
        {
            if(this->interval == interval_gap)
            {
                this->Change_wave_image();
                this->interval = 0;//归零重计
            }
        }
        else//真正的结束，可以先把所有光束去掉，然后再直接调用析构函数
        {
            for(auto i = 0; i < this->kill_wave.size() - 1; i++)
            {
                auto tmp = this->kill_wave[i];
                this->kill_wave[i] = nullptr;
                this->destory(tmp);
            }
            //if(this->gameObject != nullptr)
            this->gameObject->destory(this->gameObject);
            return;
            //delete this(十分错误的决定)//还好改了
        }
    }
    else//表示未炸，这是我们第一次渲染光波//也因此，所有的爆炸伤害都要在这里实现
    {
        //---------------以下需要实现一个归回整数原位的操作---------//
        if(this->Moving != center) //表示仍然在移动中，这是中间爆炸的场景
        {
            auto transform = this->gameObject->getComponent<ImageTransform>();
            auto this_pos = transform->pos();
            auto ld = this->belong_to->Its_map_place_ld(this_pos);
            auto lt = this->belong_to->Its_map_place_lt(this_pos);
            auto rd = this->belong_to->Its_map_place_rd(this_pos);
            auto ld_pos = this->belong_to->Get_pos(ld);
            if(ld_pos == this_pos) this->Start = ld;//没有特殊操作
            else if(ld_pos.x() == this_pos.x())
            {
                if(this->Moving == up)
                {
                    this->Start = lt;
                    transform->setPos(this->belong_to->Get_pos(lt));
                }
                else if(this->Moving == down)
                {
                    this->Start = ld;
                    transform->setPos(this->belong_to->Get_pos(ld));
                }
                else Q_ASSERT(this->Moving == center);
            }
            else if(ld_pos.y() == this_pos.y())
            {
                if(this->Moving == left)
                {
                    this->Start = ld;
                    transform->setPos(this->belong_to->Get_pos(ld));
                }
                else if(this->Moving == right)
                {
                    this->Start = rd;
                    transform->setPos(this->belong_to->Get_pos(rd));
                }
                else Q_ASSERT(this->Moving == center);
            }
            else Q_ASSERT(ld_pos == this_pos);//同样，按理来说没有出现的可能性
            this->Moving = center;
        }
        //----------------------end_here----------------------//
        this->bombing = true;//用一个bool来保证这个操作只有一次
        this->interval = 0;//同样地，初始化没有问题
        if(this->master != nullptr)
            this->master->getComponent<Bomb_Control>()->Back_a_bomb();
        this->belong_to->Delete_bomb(this->gameObject);
        auto start_index = 0;
        bool Have_down_last = true;
        QPoint pivot(0, 0);
        for(int move_dir = 0; move_dir < 4; move_dir++)
        {
            switch(Dir(move_dir))
            {
            case left:
                pivot = dir_left;
                break;
            case right:
                pivot = dir_right;
                break;
            case up:
                pivot = dir_up;
                break;
            case down:
                pivot = dir_down;
                break;
            default:
                assert(move_dir == 4);
                break;
            }
            for(int i = 1; i <= this->Bomb_strength; i++)
            {
                if(this->belong_to->Check_have_Wall(pivot * i + this->Start))
                {
                    if(this->belong_to->Check_Wall_hitable(pivot * i + this->Start))//这种情况下就不用“杀人”了
                    {
                        this->belong_to->hit_wall(pivot * i + this->Start);
                        auto edge_deam = new GameObject();
                        edge_deam->addComponent
                        (new ImageTransform(this->belong_to->Get_pos(pivot * i + this->Start)));
                        edge_deam->getComponent<ImageTransform>()->setZValue
                        (skewing + this->belong_to->Z_this_value(pivot * i + this->Start));
                        edge_deam->getComponent<ImageTransform>()->setAlignment(Qt::AlignBottom);
                        edge_deam->getComponent<ImageTransform>()->setImage(this->get_image_bound_full(Dir(move_dir)));
                        kill_wave.push_back(edge_deam);
                        if(move_dir < up)//left || right
                            start_index++;
                        this->belong_to->Get_attached(edge_deam);
                        this->Edge_and_center[move_dir] = this->kill_wave.size() - 1;
                        if(this->master != nullptr)
                        {
                            auto control = this->master->getComponent<Move_Control>();
                            control->Adding_score(Bombing_soft_wall_score);
                        }
                        break;
                    }
                    else
                    {
                        if(i < 2)
                        {
                            if(move_dir > up) //down
                                Have_down_last = false;
                            break;
                        }
                        else
                        {
                            auto last_edge = this->kill_wave[kill_wave.size() - 1]
                                             ->getComponent<ImageTransform>();
                            last_edge->setImage(this->get_image_bound_full(Dir(move_dir)));
                            this->Edge_and_center[move_dir] =
                                this->kill_wave.size() - 1; //可以直接当索引使用
                            break;
                        }
                    }
                }
                else
                {
                    this->belong_to->Check_whether_to_hit_person
                    (this->this_kill, pivot * i + this->Start);
                    if(i == this->Bomb_strength)
                    {
                        auto edge_deam = new GameObject();
                        edge_deam->addComponent
                        (new ImageTransform(this->belong_to->Get_pos(pivot * i + this->Start)));
                        edge_deam->getComponent<ImageTransform>()->setZValue
                        (skewing + this->belong_to->Z_this_value(pivot * i + this->Start));
                        edge_deam->getComponent<ImageTransform>()->setAlignment(Qt::AlignBottom);
                        edge_deam->getComponent<ImageTransform>()->setImage(this->get_image_bound_full(Dir(move_dir)));
                        kill_wave.push_back(edge_deam);
                        if(move_dir < up)//left || right
                            start_index++;
                        this->belong_to->Get_attached(edge_deam);
                        this->Edge_and_center[move_dir] = this->kill_wave.size() - 1;
                        break;
                    }
                    else//不是边缘
                    {
                        auto center_deam = new GameObject();
                        center_deam->addComponent
                        (new ImageTransform(this->belong_to->Get_pos(pivot * i + this->Start)));
                        center_deam->getComponent<ImageTransform>()->setZValue
                        (skewing + this->belong_to->Z_this_value(pivot * i + this->Start));
                        center_deam->getComponent<ImageTransform>()->setAlignment(Qt::AlignBottom);
                        center_deam->getComponent<ImageTransform>()->setImage(this->get_image_center(Dir(move_dir)));
                        kill_wave.push_back(center_deam);
                        if(move_dir < up)//left || right
                            start_index++;
                        this->belong_to->Get_attached(center_deam);
                        continue;
                    }
                }
                assert(!this->kill_wave[this->kill_wave.size() - 1]
                       ->getComponent<ImageTransform>()->getImage().isNull());
            }
        }
        //center 情况单独考虑
        if(Have_down_last)
        {
            auto Down_z_value = this->kill_wave[this->kill_wave.size() - 1]->getComponent<ImageTransform>()->zValue();
            for(int i = start_index; i < this->kill_wave.size(); i++)
                this->kill_wave[i]->getComponent<ImageTransform>()->setZValue(Down_z_value);
            this->belong_to->Check_whether_to_hit_person(this->this_kill, this->Start);
            auto change_wave = this->gameObject;
            change_wave->getComponent<ImageTransform>()->setImage(center_1);
            change_wave->getComponent<ImageTransform>()->setZValue(Down_z_value);
            kill_wave.push_back(change_wave);
            this->Edge_and_center[int(center)] = this->kill_wave.size() - 1;
            this->belong_to->Really_kill(this->this_kill, this->master);
        }
        else//下方向没有光波时，中心的光波的zvalue应该是最大的
        {
            this->belong_to->Check_whether_to_hit_person(this->this_kill, this->Start);
            auto change_wave = this->gameObject;
            change_wave->getComponent<ImageTransform>()->setImage(center_1);
            kill_wave.push_back(change_wave);
            this->Edge_and_center[int(center)] = this->kill_wave.size() - 1;
            this->belong_to->Really_kill(this->this_kill, this->master);
            auto Down_z_value = this->kill_wave[this->kill_wave.size() - 1]->getComponent<ImageTransform>()->zValue();
            for(int i = start_index; i < this->kill_wave.size(); i++)
                this->kill_wave[i]->getComponent<ImageTransform>()->setZValue(Down_z_value);
        }
    }//上下左右均需涉及到
}

const QPoint Bomb_attach::Give_start()
{
    return this->Start;
}
bool Bomb_attach::Check_master_equal(GameObject *is_this_master)
{
    return is_this_master == this->master;
}
void Bomb_attach::reset_zero_master()
{
    this->master = nullptr;
}
void Bomb_attach::onFirstUpdate()
{
    auto transform = this->gameObject->getComponent<ImageTransform>();
    assert(transform != nullptr);
    transform->setPos(this->belong_to->Get_pos(this->Start));
    return;
}
void Bomb_attach::onUpdate(float deltaTime)
{
    this->interval++;
    auto transform = this->gameObject->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    this->Bombing_left_t -= deltaTime;
    if(this->Bombing_left_t > 0)
    {
        this->Check_and_move(deltaTime);
        if(this->Bombing_left_t < Ready_to_bombing)//在准备结束的时候就不需要变化了//唯一需要考虑的就是参数的设置问题
            transform->setImage(bombing_path_over);//现在的设置效果很不错
        else if(interval == interval_gap)
        {
            interval = 0;//归零重新计数
            switch(this->Now_bomb_state)
            {
            case normal:
                transform->setImage(bombing_path_change);
                this->Now_bomb_state = change;
                break;
            case change:
                transform->setImage(bombing_path_normal);
                this->Now_bomb_state = normal;
                break;
            default:
                Q_ASSERT(this->Now_bomb_state == over);
                break;
            }//这样形成一个闭环不断变换可以模拟一个动画的效果
        }
    }
    else this->Bombing_and_over(deltaTime);

}
const QPoint Bomb_attach::Get_start()
{
    return this->Start;
}
const int Bomb_attach::Getstrength()
{
    return this->Bomb_strength;
}
