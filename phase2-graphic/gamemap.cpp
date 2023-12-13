#include "gamemap.h"


#include <cassert>
#include <cmath>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QApplication>
#include <windows.h>


#include <health.h>
#include <move_control.h>
#include <bomb_control.h>
#include <game_control.h>
#include <bomb.h>
#include <transformbuilder.h>

Gamemap::Gamemap(): Jay_music(nullptr), Priority_check(NULL) {}
void Gamemap::Set_game_scene(GameScene *needed_one)
{
    this->gamescene = needed_one;
    this->top_left = this->gamescene->sceneRect().topLeft();
    this->top_left += Excur;
    this->ending = false;
    return;
}
void Gamemap::Setmusic(QMediaPlayer *Gotton)
{
    this->Jay_music = Gotton;
}
void Gamemap::Get_attached(GameObject *attach)
{
    this->gamescene->attachGameObject(attach);
}
void Gamemap::Get_dettached(GameObject *detach)
{
    this->gamescene->detachGameObject(detach);
}
void Gamemap::Get_bomb(GameObject *A_bomb)
{
    this->Attached_bombs.append(A_bomb);
}
void Gamemap::Delete_bomb(GameObject *A_bomb)
{
    this->Attached_bombs.removeAll(A_bomb);
}
void Gamemap::Auxili_addPri(int x, int y, int Added)
{
    Priority_check[x - 1][y].priority += Added;
    Priority_check[x + 1][y].priority += Added;
    Priority_check[x][y - 1].priority += Added;
    Priority_check[x][y + 1].priority += Added;
}
void Gamemap::Auxili_addPri(QPoint place, int Added)
{
    return Auxili_addPri(place.x(), place.y(), Added);
}
void Gamemap::Check_now_memory(GameObject *master)
{
    int AImaster = 2;//两个机器人之一
    if(master == Players[3])
        AImaster = 3;
    //------------自创二进制权重分配法-----------//
    for(int i = 0; i < c_size; i++)
        for(int j = 0; j < l_size; j++)
        {
            Priority_check[i][j].priority = 0;
            Priority_check[i][j].NowCost = 99;//随便一个大值即可，我们要搜索并记录的是这一个点的最小值
        }
    //------------initialization-----------//
    //------------next_check_the_softwall-------------//
    for(int i = 0; i < c_size ; i++)
        for(int j = 0; j < l_size ; j++)
            if(Check_have_Wall(i, j) && Check_Wall_hitable(i, j))
                Auxili_addPri(i, j, 1);
    //-----------如果旁边有墙那么就标记加一，其实为最低的加分优先级--------//
    //------------next_check_players-------------//
    for(int i = 0; i < Players.size(); i++)
        if(i != AImaster)
            if(rand() % 100 < 36)
                if(Players[i] != nullptr)
                {
                    auto transform = Players[i]->getComponent<ImageTransform>();
                    auto position = transform->pos();
                    auto ld_place = this->Its_map_place_ld(position);
                    auto ld_pos = this->Get_pos(ld_place);
                    if(ld_pos == position)
                    {
                        Priority_check[ld_place.x()][ld_place.y()].priority += 2;
                        Auxili_addPri(ld_place, 2);
                    }
                    else if(position.x() == ld_pos.x())//&&position.y() != ld_pos.y()
                    {
                        Auxili_addPri(ld_place, 2);
                        Auxili_addPri(ld_place + dir_up, 2);
                    }//实际上这种中间镂空的增权设计与双位置附近增权配合的真是天衣无缝
                    else if(position.y() == ld_pos.y())//&&position.x() != ld_pos.x()
                    {
                        Auxili_addPri(ld_place, 2);
                        Auxili_addPri(ld_place + dir_right, 2);
                    }
                    else Q_ASSERT(position.y() == ld_pos.y() || position.x() == ld_pos.x());//实际上在特殊的控制下这种情况不会出现
                }
    //------------加分优先级为权重2-------------//
    //------------next_check_props-------------//
    for(int i = 0; i < c_size ; i++)
        for(int j = 0; j < l_size ; j++)
            if(this->Items_map[i][j] != nullptr)
                Priority_check[i][j].priority += 4;
    //------------加分优先级为权重4-------------//
    //------------last_check_the_safety-------------//
    for(auto bomb : Attached_bombs)
    {
        auto attach = bomb->getComponent<Bomb_attach>();
        auto start = attach->Get_start();
        auto strength = attach->Getstrength();
        for(int cor = left; cor < center; cor++)
            for(int now = 1; now <= strength; now++)
            {
                Priority_check[(now * AllDir[cor] + start).x()][(now * AllDir[cor] + start).y()].priority -= 8;//危险的减去8就等价于安全的加上8
                if(this->Check_have_Wall(now * AllDir[cor] + start))
                    break;
            }//而且，其实我们完全没有必要考虑是否可走，不可走的话在寻路搜索时就已经排除了
        Priority_check[start.x()][start.y()].priority -= 8;//最后减去中间即可
    }
    //------------加分优先级为权重4-------------//
}

QPoint Gamemap::Its_map_place_lt(const QPointF F_place)
{
    QPoint tmp;
    qreal our_y = F_place.y() - this->top_left.y();
    qreal our_x = F_place.x() - this->top_left.x();
    tmp.rx() = std::floor(our_y / pixblock);
    tmp.ry() = std::floor(our_x / pixblock);
    Q_ASSERT(tmp.x() >= 0 && tmp.y() >= 0);
    return tmp;
}
QPoint Gamemap::Its_map_place_ld(const QPointF F_place)
{
    QPoint tmp;
    qreal our_y = F_place.y() - this->top_left.y();
    qreal our_x = F_place.x() - this->top_left.x();
    tmp.rx() = std::ceil(our_y / pixblock);
    tmp.ry() = std::floor(our_x / pixblock);
    Q_ASSERT(tmp.x() >= 0 && tmp.y() >= 0);
    return tmp;
}
QPoint Gamemap::Its_map_place_rt(const QPointF F_place)
{
    QPoint tmp;
    qreal our_y = F_place.y() - this->top_left.y();
    qreal our_x = F_place.x() - this->top_left.x();
    tmp.rx() = std::floor(our_y / pixblock);
    tmp.ry() = std::ceil(our_x / pixblock);
    Q_ASSERT(tmp.x() >= 0 && tmp.y() >= 0);
    return tmp;
}
QPoint Gamemap::Its_map_place_rd(const QPointF F_place)
{
    QPoint tmp;
    qreal our_y = F_place.y() - this->top_left.y();
    qreal our_x = F_place.x() - this->top_left.x();
    tmp.rx() = std::ceil(our_y / pixblock);
    tmp.ry() = std::ceil(our_x / pixblock);
    Q_ASSERT(tmp.x() >= 0 && tmp.y() >= 0);
    return tmp;
}
QPoint Gamemap::Its_map_place_in(const QPointF F_place)
{
    auto tmp = this->Its_map_place_ld(F_place);
    auto tmp_pos = this->Get_pos(tmp);
    if(tmp_pos.x() == F_place.x())
    {
        QPoint tmp_2(tmp.x() - 1, tmp.y());
        auto tmp_pos_2 = this->Get_pos(tmp_2);
        if(F_place.y() - tmp_pos_2.y() > tmp_pos.y() - F_place.y())
            return tmp;
        else return tmp_2;
    }
    else if(tmp_pos.y() == F_place.y())
    {
        QPoint tmp_2(tmp.x(), tmp.y() + 1);
        auto tmp_pos_2 = this->Get_pos(tmp_2);
        if(F_place.x() - tmp_pos.x() > tmp_pos_2.x() - F_place.x())
            return tmp_2;
        else return tmp;
    }
    else Q_ASSERT(tmp_pos.x() == F_place.y() || tmp_pos.y() == F_place.y());
    return tmp;
}
QPointF Gamemap::Get_pos(int first, int second)
{
    return this->Ground_map[first][second]->getComponent<ImageTransform>()->pos();
}
QPointF Gamemap::Get_pos(QPoint int_place)
{
    return this->Get_pos(int_place.x(), int_place.y());
}
const QPointF Gamemap::Get_topleft()
{
    return this->top_left;
}
qreal Gamemap::Z_this_value(int first, int second)
{
    //Q_ASSERT(this->Wall_map[first][second] != nullptr);
    return this->Wall_map[first][0]->display_zValue();
}//因为每行都是一样的，所以直接用第一个就可以
qreal Gamemap::Z_this_value(QPoint int_place)
{
    return this->Z_this_value(int_place.x(), int_place.y());
}
void Gamemap::ChangeSearchWay(int x, int y, int now_cost, Dir now_choice,
                              int &biggest_priority, int &Leastcost, Dir &Bestchoice)
{
    if(now_cost > Priority_check[x][y].NowCost)return;//剩下的情况就只有小于等于了
    Priority_check[x][y].NowCost = now_cost;
    if(Priority_check[x][y].priority > biggest_priority ||
            Priority_check[x][y].priority == biggest_priority && now_cost < Leastcost)
    {
        Bestchoice = now_choice;
        Leastcost = now_cost;
        biggest_priority = Priority_check[x][y].priority;
    }
    QPoint nowPlace(x, y);
    for(int cor = left; cor < center; cor++)
    {
        if(Check_cannot_go(nowPlace + AllDir[cor]))continue;
        //以下是可走的方案
        if(now_choice == center)
        {
            Q_ASSERT(now_cost == 0);
            ChangeSearchWay(nowPlace + AllDir[cor], now_cost + 1, (Dir)cor, biggest_priority, Leastcost, Bestchoice);
        }
        else //这里的逻辑是只需要控制第一步，之后的扫到就会终止
            ChangeSearchWay(nowPlace + AllDir[cor], now_cost + 1, now_choice, biggest_priority, Leastcost, Bestchoice);
    }
    return;
}
void Gamemap::ChangeSearchWay(QPoint thisplace, int now_cost, Dir now_choice,
                              int &biggest_priority, int &Leastcost, Dir &Bestchoice)
{
    return this->ChangeSearchWay(thisplace.x(), thisplace.y(), now_cost, now_choice,
                                 biggest_priority, Leastcost, Bestchoice);
}
Dir Gamemap::Give_computer_a_dir(GameObject *master)
{
    auto transform = master->getComponent<ImageTransform>();
    auto f_place = transform->pos();
    this->Check_now_memory(master);
    auto ld_place = Its_map_place_ld(f_place);
    auto ld_pos = Get_pos(ld_place);
    Q_ASSERT(ld_pos == f_place);
    Dir Bestchoice = center;
    int biggest_priority = Priority_check[ld_place.x()][ld_place.y()].priority;
    int Leastcost = 0;
    ChangeSearchWay(ld_place, 0, center, biggest_priority, Leastcost, Bestchoice);
    return Bestchoice;
}
bool Gamemap::put_bomb_or_not(GameObject *master)
{
    auto transform = master->getComponent<ImageTransform>();
    auto f_place = transform->pos();
    auto int_place = Its_map_place_ld(f_place);
    this->Check_now_memory(master);
    auto attach = master->getComponent<Bomb_Control>();
    auto strength = attach->How_strong() + 1;
    for(int cor = left; cor < center; cor++)
        for(int now = 1; now <= strength; now++)
        {
            Priority_check[(now * AllDir[cor] + int_place).x()][(now * AllDir[cor] + int_place).y()].priority -= 8;//危险的减去8就等价于安全的加上8
            if(this->Check_have_Wall(now * AllDir[cor] + int_place))
                break;
        }
    auto ld_place = Its_map_place_ld(f_place);
    auto ld_pos = Get_pos(ld_place);
    Q_ASSERT(ld_pos == f_place);
    Dir Bestchoice = center;
    int biggest_priority = Priority_check[ld_place.x()][ld_place.y()].priority;
    int Leastcost = 0;
    ChangeSearchWay(ld_place, 0, center, biggest_priority, Leastcost, Bestchoice);
    if(biggest_priority < 0) //代表放下炸弹不能找到安全区域
        return false;//生存仍然是机器人的第一需求


    for(auto cor = 0; cor < center; cor++)//这个检测算是卫士＋真正的目的
        if(this->Check_have_Wall(int_place + AllDir[cor]) && this->Check_Wall_hitable(int_place + AllDir[cor]))
            return true;
    for(auto other_player : Players)
    {
        if(other_player == nullptr)continue;
        if(other_player == master)
            continue;//不考虑自己是最基本的要求
        auto Otransform = other_player->getComponent<ImageTransform>();
        auto Of_place = Otransform->pos();
        auto ld_place = Its_map_place_ld(Of_place);
        //auto ld_pos = Get_pos(ld_place);
        for(int cor = left; cor < center; cor++)
            if(ld_place + dir_up - int_place == AllDir[cor])return true;
        for(int cor = left; cor < center; cor++)
            if(ld_place + dir_right - int_place == AllDir[cor])return true;
        for(int cor = left; cor < center; cor++)//直接一起考虑算了，反正💣多多益善
            if(ld_place - int_place == AllDir[cor])return true;
    }
    return false;
}//周围有软墙就放
bool Gamemap::Check_have_Wall(int first, int second)
{
    auto this_place = this->Wall_map[first][second];
    if(this_place == nullptr)
        return false;
    else return this_place->Cannot_go();
}
bool Gamemap::Check_have_Wall(QPoint int_place)
{
    return this->Check_have_Wall(int_place.x(), int_place.y());
}
bool Gamemap::Check_Wall_hitable(int first, int second)
{
    Q_ASSERT(this->Check_have_Wall(first, second));
    return !this->Wall_map[first][second]->Itis_hard();
}
bool Gamemap::Check_Wall_hitable(QPoint int_place)
{
    return Check_Wall_hitable(int_place.x(), int_place.y());
}
bool Gamemap::Check_have_bomb(QPoint int_place)
{
    bool check_all = false;
    if(this->Attached_bombs.empty()) return false;
    else for(auto A_bomb : this->Attached_bombs)
        {
            if(A_bomb->getComponent<Bomb_attach>()->Give_start() == int_place)
            {
                check_all = true;
                break;
            }
        }
    return check_all;
}
bool Gamemap::Check_have_bomb(int first, int second)
{
    QPoint tmp(first, second);
    return this->Check_have_bomb(tmp);
}
bool Gamemap::Check_cannot_go(QPoint int_place)
{
    return Check_have_bomb(int_place) || this->Check_have_Wall(int_place);
}
bool Gamemap::Check_cannot_go(int first, int second)
{
    QPoint tmp(first, second);
    return this->Check_cannot_go(tmp);
}

const char* Gamemap::GetdisPic(Image_body_plan thisone)
{
    switch(thisone)
    {
    case _RED_:
        return "../pic-use/state-display/red.png";
        break;
    case _BLUE_:
        return "../pic-use/state-display/blue.png";
        break;
    case _GREEN_:
        return "../pic-use/state-display/green.png";
        break;
    case _PINK_:
        return "../pic-use/state-display/pink.png";
        break;
    case _YELLOW_:
        return "../pic-use/state-display/yellow.png";
        break;
    case _PURPLE_:
        return "../pic-use/state-display/purple.png";
        break;
    case _Error_:
    default:
        Q_ASSERT(thisone == _RED_);
        return " ";//错误情况
    }
    return " ";//错误情况
}

void Gamemap::Set_background()
{
    auto back_ground = new GameObject();
    back_ground->addComponent(new ImageTransform(this->gamescene->sceneRect().topLeft()));
    back_ground->getComponent<ImageTransform>()->setImage("../pic-use/others/back_ground.png");
    back_ground->getComponent<ImageTransform>()->setZValue(-1);
    this->Get_attached(back_ground);
}
void Gamemap::Init_single_land(GameObject * A_land, int high_add)
{
    auto transform = A_land->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    transform->setAlignment(Qt::AlignBottom);
    transform->setZValue(land_fixed + high_add);
}//此函数用于将所有的陆地状况初始化
void Gamemap::double_pool_init(int first, int second)
{
    Q_ASSERT(this->Wall_map.size() == c_size);
    for(int i = 0; i < c_size; i++)
        Q_ASSERT(this->Wall_map[i].size() == l_size);
    // after init we need to guarantee that we can get something whenever we take it
    Q_ASSERT(this->Wall_map[first][second]->Itis_hard());
    Q_ASSERT(this->Wall_map[first][second + 1]->Itis_hard());
    auto this_pos = this->Ground_map[first][second]->getComponent<ImageTransform>()->pos();
    auto Double_change = new ImageTransform(this_pos);
    auto getrand = rand() % 2;
    if(getrand)
        Double_change->setImage("../pic-use/hard/d1.png");
    else
        Double_change->setImage("../pic-use/hard/d2.png");
    Double_change->setAlignment(Qt::AlignBottom);
    Double_change->setZValue(Ground_map[first][second]->getComponent<ImageTransform>()->zValue());
    this->Wall_map[first][second]->Change_image(Double_change);
    this->Wall_map[first][second + 1]->Change_image(Double_change);
}
void Gamemap::Coffin_init(int first, int second)
{
    Q_ASSERT(this->Wall_map.size() == c_size);
    for(int i = 0; i < c_size; i++)
        Q_ASSERT(this->Wall_map[i].size() == l_size);
    // after init we need to guarantee that we can get something whenever we take it
    auto this_pos = this->Ground_map[first][second]->getComponent<ImageTransform>()->pos();
    auto coffin = new ImageTransform(this_pos);
    coffin->setImage("../pic-use/hard/bill.png");
    coffin->setAlignment(Qt::AlignBottom);
    coffin->setZValue(this->Ground_map[first][second]->getComponent<ImageTransform>()->zValue());
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 2; j++)
        {
            Q_ASSERT(this->Wall_map[first - i][second + j]->Itis_hard());
            this->Wall_map[first - i][second + j]->Change_image(coffin);
        }
}

void Gamemap::Add_one_player(int first, int second, Control_type this_control, int blood_num, Image_body_plan this_plan)
{
    auto thisplayer = new GameObject();
    QPointF this_pos = Ground_map[first][second]->getComponent<ImageTransform>()->pos();
    thisplayer->addComponent(new ImageTransform(this_pos));
    //thisplayer->getComponent<ImageTransform>()->setZValue(normal_fixed);
    thisplayer->getComponent<ImageTransform>()->setAlignment(Qt::AlignBottom);
    auto control_this_move = new Move_Control(this_control, this, this_plan);
    thisplayer->addComponent(control_this_move);
    auto control_this_bomb = new Bomb_Control(this_control, this);
    thisplayer->addComponent(control_this_bomb);
    thisplayer->addComponent(new Health(Init_blood));
    this->gamescene->attachGameObject(thisplayer);
    this->Players.push_back(thisplayer);
    return;
}
void Gamemap::Add_one_player(QPoint init_place, Control_type this_control, int blood_num, Image_body_plan this_plan)
{
    this->Add_one_player(init_place.x(), init_place.y(), this_control, blood_num, this_plan);
    return;
}
void Gamemap::Add_player_init()
{
    this->Add_one_player(Init_player_place_1, First_key_type, 2, _RED_);
    this->Add_one_player(Init_player_place_2, Second_key_type, 2, _BLUE_);
    this->Add_one_player(Init_player_place_3, Computer_type, 2, _PURPLE_);
    this->Add_one_player(Init_player_place_4, Computer_type, 2, _GREEN_);
    Q_ASSERT(this->Players.size() == 4);
    return;
}
void Gamemap::Adding_init_button()
{
    auto stop_button = new GameObject();
    stop_button->addComponent(new ImageTransform(this->top_left + ButtonStop));
    stop_button->addComponent(new control_game(stop));
    this->gamescene->attachGameObject(stop_button);
    auto end_button = new GameObject();
    end_button->addComponent(new ImageTransform(this->top_left + ButtonEnd));
    end_button->addComponent(new control_game(end));
    this->gamescene->attachGameObject(end_button);
}
void Gamemap::Special_Com_init()   //ASSERT 有备无患
{
    Q_ASSERT(this->Wall_map.size() == 15);
    for(int i = 0; i < 15; i++)
        Q_ASSERT(this->Wall_map[i].size() == 20);
    // after init we need to guarantee that we can get something whenever we take it
    this->Coffin_init(8, 7);
    this->Coffin_init(8, 9);
    this->Coffin_init(8, 11);
    //First Changing over here, we can have three coffin for the losers()
    //let's have a simple try first//successfuly done
    this->double_pool_init(2, 5);//this->double_pool_init(4, 8);
    this->double_pool_init(12, 5);//this->double_pool_init(10, 10);
    this->double_pool_init(5, 3);//现在设定如此，如果向增删直接添删函数应用即可。
    this->double_pool_init(9, 3);
    this->double_pool_init(5, 15);
    this->double_pool_init(9, 15);
    this->double_pool_init(2, 13);
    this->double_pool_init(12, 13);//Second Changing over here, replace some double hard walls as a pool
//这些变化是人为控制的，可以想象如果要有多张地图，我们可以设置多种特殊化方式并根据选择来应用
//在可预见的未来，这些都是可以来实现的，在2022/7/1之前实现的要包含人物换皮肤的功能
}
void Gamemap::Add_init_wall()
{
    Q_ASSERT(!this->Ground_map.empty());
    Q_ASSERT(this->Wall_map.empty());
    for(int i = 0; i < c_size; i++)
    {
        this->Wall_map.push_back({});//init
        this->Items_map.push_back({});//init
    }
    Q_ASSERT(this->Wall_map.size() == 15);
    for(int i = 0; i < c_size; i++)
        for(int j = 0; j < l_size; j++)
        {
            if(Check_Wall[i][j] == _H_)
            {
                auto add_try = new Wall(this->Ground_map[i][j]->getComponent<ImageTransform>()->pos());
                add_try->Init_wall_hard(this->Ground_map[i][j]->getComponent<ImageTransform>()->zValue());
                add_try->Get_attached(this->gamescene);
                this->Wall_map[i].push_back(add_try);
            }
            else if(Check_Wall[i][j] == _S_)
            {
                auto add_try = new Wall(this->Ground_map[i][j]->getComponent<ImageTransform>()->pos());
                add_try->En_hitable();
                add_try->Init_wall_soft(this->Ground_map[i][j]->getComponent<ImageTransform>()->zValue());
                add_try->Get_attached(this->gamescene);
                this->Wall_map[i].push_back(add_try);
                //this->Wall_map[i].push_back(nullptr);//炸弹炸软墙功能未实现时实验的需要工具
            }
            else this->Wall_map[i].push_back(nullptr);
            //_N_ or _E_表示这里无墙//无墙就要加入nullptr这样满足我们对返回时的要求
            this->Items_map[i].push_back(nullptr);//初始化全部是nullptr
        }
    for(int i = 0; i < c_size; i++)
    {
        Q_ASSERT(this->Wall_map[i].size() == l_size);
    }// after init we need to guarantee that we can get something whenever we take it
    this->Special_Com_init();
}

void Gamemap::Update_states()
{
    for(int i = 0; i < player_number; i++)
    {
        for(int cor = dScores; cor <= dMax; cor++)
        {
            auto text = this->Display[i][cor];
            auto master = this->Players[i];
            text->setText(QString::number(1));
            QPointF thisex(0, 0);
            if(master == nullptr)
                switch(cor)
                {
                case dScores:
                case dBombMore:
                case dBombStronger:
                case dTreasure:
                    break;
                case dBloodNum:
                    text->setText("0");
                    break;
                case dFaster:
                    text->setText("N");
                    break;
                case dMovable:
                    text->setText("N");
                    break;
                default:
                    break;
                }
            else
            {
                auto move_control = master->getComponent<Move_Control>();
                auto health = master->getComponent<Health>();
                auto bomb_control = master->getComponent<Bomb_Control>();
                Q_ASSERT(move_control != nullptr);
                Q_ASSERT(health != nullptr);
                Q_ASSERT(bomb_control != nullptr);
                switch(cor)
                {
                case dScores:
                    text->setText(QString::number(move_control->Get_score()));
                    break;
                case dBloodNum:
                    text->setText(QString::number(health->How_healthy()));
                    break;
                case dBombMore:
                    text->setText(QString::number(bomb_control->How_many_have()));
                    break;
                case dBombStronger:
                    text->setText(QString::number(bomb_control->How_strong()));
                    break;
                case dTreasure:
                    text->setText(QString::number(move_control->GetTreasurenum()));
                    break;
                case dFaster:
                    if(move_control->if_fast())
                        text->setText("Y");
                    else text->setText("N");
                    break;
                case dMovable:
                    if(move_control->Can_MoveBomb())
                        text->setText("Y");
                    else text->setText("N");
                    break;
                default:
                    break;
                }
            }
        }
        Q_ASSERT(this->Display[i].size() == dMax + 1);
    }
    //if(Jay_music != nullptr && Jay_music->mediaStatus() == QMediaPlayer::EndOfMedia)
    //Jay_music->play();
}
void Gamemap::Display_state_for_all()
{
    for(int i = 0; i < player_number; i++)
    {
        this->Display.push_back({});
        auto startpos = this->top_left + StateDisplayS + i * AnotherPlus;
        auto dispic = new GameObject();
        dispic->addComponent(new ImageTransform(startpos));
        auto distransform = dispic->getComponent<ImageTransform>();
        distransform->setAlignment(Qt::AlignBottom);
        auto tempget = this->Players[i]->getComponent<Move_Control>();
        Q_ASSERT(tempget != nullptr);
        distransform->setImage(this->GetdisPic(tempget->Skin_plan()));
        this->gamescene->attachGameObject(dispic);
        for(int cor = dScores; cor <= dMax; cor++)
        {
            auto display = new GameObject();
            display->addComponent(new ImageTransform());
            auto transform = display->getComponent<ImageTransform>();
            auto text = new QGraphicsSimpleTextItem(transform);
            text->setBrush(Qt::white);
            QFont font = text->font();
            font.setPixelSize(fontsize);//像素大小
            font.setBold(true);
            text->setFont(font);
            text->setText(QString::number(1));//pre-set
            QPointF thisex = DisEx[cor];
            transform->setPos(startpos + thisex);
            transform->setZValue(800);
            transform->setAlignment(Qt::AlignBottom);
            this->gamescene->attachGameObject(display);
            this->Display[i].push_back(text);
        }
        Q_ASSERT(this->Display[i].size() == dMax + 1);
    }
    this->Update_states();//初始的时候就可以直接用此函数来显示状态
}

void Gamemap::Init_map()   //初始化时直接挂载，之后不再管理
{
    this->Priority_check = new way_memory*[c_size];
    for(int i = 0; i < c_size; i++)
        Priority_check[i] = new way_memory[l_size];//way_memory Priority_check[c_size][l_size];
    this->Set_background();
    srand(time(0));
    Q_ASSERT(this->Ground_map.empty());
    for(int i = 0; i < c_size; i++)
        this->Ground_map.push_back({});//it should be the size of 15 \times 20
    Q_ASSERT(this->Ground_map.size() == c_size);
    for(int i = 0; i < c_size; i++)
        for(int j = 0; j < l_size; j++)
        {
            auto land_pointer = new GameObject();
            QPointF thispos(this->top_left.x() + pixblock * j, this->top_left.y() + pixblock * i);
            land_pointer->addComponent(new ImageTransform(thispos));
            auto transform = land_pointer->getComponent<ImageTransform>();
            auto getrand = rand() % 3;
            if(getrand < 1)
                transform->setImage("../pic-use/land/l1.png");
            else if(getrand == 1)
                transform ->setImage("../pic-use/land/l2.png");
            else//Get it randomly
                transform->setImage("../pic-use/land/l3.png");
            Q_ASSERT(!land_pointer->getComponent<ImageTransform>()->getImage().isNull());
            Init_single_land(land_pointer, i);
            this->Ground_map[i].push_back(land_pointer);
            this->gamescene->attachGameObject(land_pointer);
        }
    for(int i = 0; i < c_size; i++)
        Q_ASSERT(this->Ground_map[i].size() == l_size);
    this->Add_init_wall();
    this->Add_player_init();
    this->Adding_init_button();
    this->Display_state_for_all();
    Q_ASSERT(this->Attached_bombs.empty());
}
void Gamemap::exit_game()
{
    QApplication::quit();
}
void Gamemap::Ending_game()
{
    int num = 0;
    if(ending)
    {
        this->gamescene->Stop_all();
        Sleep(200);
        this->exit_game();
    }
    for(int i = 0; i < player_number; i++)
        if(this->Players[i] != nullptr)
            if(this->Players[i]->getComponent<Move_Control>()->Get_score() >= winning)
            {
                num = i;
                auto winner = new GameObject();
                winner->addComponent(new ImageTransform(this->top_left + play_center));
                auto winner_t = winner->getComponent<ImageTransform>();
                auto text_winner = new QGraphicsSimpleTextItem(winner_t);
                text_winner->setText("Our winner is: " + QString::number(num + 1) + " !!!!"); //
                text_winner->setBrush(Qt::white);
                QFont font = text_winner->font();
                font.setPixelSize(60);  // 像素大小
                text_winner->setFont(font);
                winner_t->setZValue(800);
                this->gamescene->attachGameObject(winner);
                this->ending = true;
                return;
            }
    //check first
    for(int i = 0; i < player_number; i++)
        if(this->Players[i] == nullptr)
            num++;
    if(num > 2)
    {
        for(int i = 0; i < player_number; i++)
            if(this->Players[i] != nullptr)
                num = i;
        auto winner = new GameObject();
        winner->addComponent(new ImageTransform(this->top_left + play_center));
        auto winner_t = winner->getComponent<ImageTransform>();
        auto text_winner = new QGraphicsSimpleTextItem(winner_t);
        text_winner->setText("Our winner is: " + QString::number(num + 1) + " !!!!"); //
        text_winner->setBrush(Qt::white);
        QFont font = text_winner->font();
        font.setPixelSize(60);  // 像素大小
        text_winner->setFont(font);
        winner_t->setZValue(800);
        this->gamescene->attachGameObject(winner);
        this->ending = true;
    }
}

void Gamemap::Get_random_prop(int first, int second, int getrand)
{
    Item_check this_type = Faster_speed;
    if(getrand > -1 && getrand < 6)
    {
        this_type = Item_check(int(Faster_speed) + getrand);
        this->Items_map[first][second] = new Props
        (this_type, this->Get_pos(first, second), this->Z_this_value(first, second), this->gamescene);
    }
}
void Gamemap::Get_random_prop(QPoint int_place, int getrand)
{
    this->Get_random_prop(int_place.x(), int_place.y(), getrand);
}
void Gamemap::Item_be_gotton(int first, int second, GameObject * this_player)
{
    if(this->Items_map[first][second] != nullptr)
    {
        auto tmp_pointer = this->Items_map[first][second];
        this->Items_map[first][second] = nullptr;
        tmp_pointer->Be_gotton(this_player);
    }
}
void Gamemap::Item_be_gotton(QPoint int_place, GameObject * this_player)
{
    this->Item_be_gotton(int_place.x(), int_place.y(), this_player);
    this->Update_states();//每吃一次道具更新一次
}

void Gamemap::hit_wall(int first, int second)
{
    assert(this->Check_Wall_hitable(first, second));
    auto tmp_store = Wall_map[first][second];
    this->Wall_map[first][second] = nullptr;
    tmp_store->Be_hit();
    delete tmp_store;
    int getrand = rand() % 10;
    this->Get_random_prop(first, second, getrand);
    this->Update_states();//每一次炸墙都更新一次
    return;
}
void Gamemap::hit_wall(QPoint int_place)
{
    this->hit_wall(int_place.x(), int_place.y());
}


void Gamemap::Check_whether_to_hit_person(bool this_kill[player_number], QPoint int_place)
{
    //auto right_top_pos = this->Get_pos(first - 1, second + 1);
    for(int i = 0; i < this->Players.size(); i++)
    {
        if(this->Players[i] == nullptr) continue;
        else
        {
            auto player_transform = this->Players[i]->getComponent<ImageTransform>();
            auto player_pos = player_transform->pos();
            auto player_place_ld = this->Its_map_place_ld(player_pos);
            auto player_place_lt = this->Its_map_place_lt(player_pos);
            auto player_place_rd = this->Its_map_place_rd(player_pos);
            //auto player_place_rt = this->Its_map_place_rt(player_pos);//在地图的特殊性下不需要这个位置
            auto ld_pos = this->Get_pos(player_place_ld);
            if(ld_pos == player_pos)//分情况讨论
            {
                if((player_place_ld == int_place))
                {
                    this_kill[i] = true;
                    continue;
                }
                else continue;  //不等就相当于一定无作用。
            }
            else if(ld_pos.x() == player_pos.x())
            {
                if((player_place_ld == int_place || player_place_lt == int_place))
                {
                    this_kill[i] = true;
                    continue;
                }
                else continue;  //不等就相当于一定无作用。
            }
            else if(ld_pos.y() == player_pos.y())
            {
                if((player_place_ld == int_place || player_place_rd == int_place))
                {
                    this_kill[i] = true;
                    continue;
                }
                else continue;  //不等就相当于一定无作用。
            }
            else//不等就相当于一定无作用。
                continue;
        }
    }
    //清除需要统一进行
    Q_ASSERT(this->Players.size() == player_number);
}
void Gamemap::Really_kill(bool this_kill[player_number], GameObject * master)
{
    for(int i = 0; i < player_number; i++)
    {
        if(this_kill[i] && this->Players[i] != nullptr)
        {
            auto health = this->Players[i]->getComponent<Health>();
            if(health->beHit())
            {
                for(auto items : this->Attached_bombs)
                {
                    if(items->getComponent<Bomb_attach>()->Check_master_equal(this->Players[i]))
                        items->getComponent<Bomb_attach>()->reset_zero_master();
                }
                if(master != nullptr)
                {
                    auto control = master->getComponent<Move_Control>();
                    if(master != this->Players[i])
                        control->Adding_score(Kill_people_adding_score);
                }
                auto tmp = this->Players[i];
                this->Players[i] = nullptr;
                tmp->destory(tmp);
            }
            else
            {
                if(master != nullptr)
                {
                    auto control = master->getComponent<Move_Control>();
                    if(master != this->Players[i])
                        control->Adding_score(Bombing_people_add_score);
                }
            } //this->Check_map_good();
        }
        else continue;
    }
    this->Update_states();
    this->Ending_game();
}
void Gamemap::Set_bomb_dir(int first, int second, Dir set_this)
{
    QPoint tmp(first, second);
    this->Set_bomb_dir(tmp, set_this);
}
void Gamemap::Set_bomb_dir(QPoint int_place, Dir set_this)
{
    Q_ASSERT(!this->Attached_bombs.empty());
    for(auto A_bomb : this->Attached_bombs)
        if(A_bomb->getComponent<Bomb_attach>()->Give_start() == int_place)
            A_bomb->getComponent<Bomb_attach>()->Set_dir_move(set_this);
}


void Gamemap::Check_map_good()
{
    std::cout << "Ground_map here now station is::" << std::endl;
    std::cout << this->Ground_map.size() << std::endl;
    for(int i = 0; i < c_size; i++)
        if(this->Ground_map[i].size() == l_size)
            std::cout << "good " << i << " ";
    std::cout << std::endl;
    std::cout << "Wall_map here now station is::" << std::endl;
    std::cout << this->Wall_map.size() << std::endl;
    for(int i = 0; i < c_size; i++)
        if(this->Wall_map[i].size() == l_size)
            std::cout << "good " << i << " ";
    std::cout << std::endl;
    std::cout << "Items_map here now station is::" << std::endl;
    std::cout << this->Items_map.size() << std::endl;
    for(int i = 0; i < c_size; i++)
        if(this->Items_map[i].size() == l_size)
            std::cout << "good " << i << " ";
    std::cout << std::endl;
    for(int i = 0; i < player_number; i++)
        if(this->Players[i] == nullptr)
            std::cout << "player " << i << "killed" << std::endl;
        else std::cout << "player " << i << "still alive" << std::endl;
}//only for checking
