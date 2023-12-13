#ifndef _GAME_MAP_H_
#define _GAME_MAP_H_
//此类为软硬墙的同一个类，一旦我们需要软墙时就只要挂载hittable就可以了
#include <vector>
#include <queue>


#include <constant_set.h>
#include <gameobject.h>
#include <imagetransform.h>
#include <utility>
#include <gamescene.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <wall.h>
#include <props.h>


class Gamemap
{
public:
    Gamemap();
public://fundamental functions
    void Set_game_scene(GameScene *needed_one);
    void Setmusic(QMediaPlayer *Gotton);
    void Get_attached(GameObject *attach);
    void Get_dettached(GameObject *detach);
    void Get_bomb(GameObject *A_bomb);
    void Delete_bomb(GameObject *A_bomb);
    void Auxili_addPri(int x, int y, int Added);
    void Auxili_addPri(QPoint place, int Added);
    void Check_now_memory(GameObject *master);//实际上标注优先级的时候不能加上自身影响
    QPointF Get_pos(int first, int second);
    QPointF Get_pos(QPoint int_place);
    const QPointF Get_topleft();
public://---important-change from real numbers to intergers
    QPoint Its_map_place_lt(const QPointF F_place);
    QPoint Its_map_place_ld(const QPointF F_place);
    QPoint Its_map_place_rt(const QPointF F_place);
    QPoint Its_map_place_rd(const QPointF F_place);
    QPoint Its_map_place_in(const QPointF F_place);

    qreal Z_this_value(int first, int second);
    qreal Z_this_value(QPoint int_place);
private:
    void ChangeSearchWay(int x, int y,  int now_cost, Dir now_choice,
                         int &biggest_priority, int &Leastcost, Dir &Bestchoice);
    void ChangeSearchWay(QPoint thisplace, int now_cost, Dir now_choice,
                         int &biggest_priority, int &Leastcost, Dir &Bestchoicee);
public:
    Dir Give_computer_a_dir(GameObject *master);
    bool put_bomb_or_not(GameObject *master);
//--------------------------------------------------//
public:
    bool Check_have_Wall(int first, int second);
    bool Check_have_Wall(QPoint int_place);
    bool Check_Wall_hitable(int first, int second);
    bool Check_Wall_hitable(QPoint int_place);
    bool Check_have_bomb(QPoint int_place);
    bool Check_have_bomb(int first, int second);
    bool Check_cannot_go(QPoint int_place); //不能走就返回true，反之为false
    bool Check_cannot_go(int first, int second);
private:
    const char* GetdisPic(Image_body_plan thisone);

private:
    void Set_background();
    void Init_single_land(GameObject *A_land, int high_add);
    void double_pool_init(int first, int second);
    void Coffin_init(int first, int second);
//这几个都是只在类内调用的专门方便实现的函数，可以直接定义为私有
private:
    void Add_one_player(int first, int second, Control_type this_control, int blood_num, Image_body_plan this_plan);
    void Add_one_player(QPoint init_place, Control_type this_control, int blood_num, Image_body_plan this_plan);
    void Add_player_init();
    void Adding_init_button();
private:
    void Get_random_prop(int first, int second, int getrand);
    void Get_random_prop(QPoint int_place, int getrand);//更新时实时地进行一些更新道具
private:
    void Special_Com_init();
    void Add_init_wall();
public:
    void Update_states();
private:
    void Display_state_for_all();
//---------------------------------------//
//------/除了下边这个真正调用的“总”函数\-----//
public:
    void Init_map();
    void exit_game();
    void Ending_game();
//--------------------------------------------------//
public:
    void Item_be_gotton(int first, int second, GameObject *this_player);
    void Item_be_gotton(QPoint int_place, GameObject *this_player);
    void hit_wall(int first, int second);
    void hit_wall(QPoint int_place);
public:
    void Check_whether_to_hit_person(bool this_kill[player_number], QPoint int_place); //具体做法是检查地图上当前所有的人物，若在界内则消除
    void Really_kill(bool this_kill[player_number], GameObject *master);
public://炸弹移动实现
    void Set_bomb_dir(int first, int second, Dir set_this);
    void Set_bomb_dir(QPoint int_place, Dir set_this);
public://check_itself
    void Check_map_good();

protected:
    GameScene *gamescene = nullptr;
private:
    QMediaPlayer *Jay_music;
    std::vector<std::vector<GameObject *>> Ground_map;
    //在这里想用vector来处理的原因主要是寻求一个共同的组织，要找标准位置等时都会更加方便
    std::vector<std::vector<Wall *>> Wall_map;
    //在人物行走的时候用这个来标记是否有墙，如果没有返回nullptr
    std::vector<std::vector<Props *>> Items_map;
    //同样用来标记有无道具，有的话达到可以直接产生相应的效果，没有的话就返回nullptr
    QList<GameObject *> Attached_bombs;
    //存放炸弹，作用：1.判断是否可行走
    //2.最重要的是可以用此来标记可生存地点，用来完成机器人寻路算法，其实这样看来Items_map也是相同的道理
    //std::priority_queue<Bomb> Bomb_still_here;
    //实际上我们有了游戏现有的更新策略，就不再需要这样的数据结构来进行控制了
private:
    way_memory **Priority_check;//int Priority_check[c_size][l_size];
private:
    std::vector<GameObject *> Players;//[0][1][2][3]共四个玩家
    QPointF top_left;//虽然有getGameObject(const char *name);这一函数，但是从实现来看太慢太浪费时间了
    bool ending;
private://Now_fixed
    std::vector<std::vector<QGraphicsSimpleTextItem *>> Display;
};

#endif  // _MAP_H_
