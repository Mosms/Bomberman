#ifndef __BOMB_H_
#define __BOMB_H_

#include <constant_set.h>

#include <physics.h>
#include <component.h>
#include <gameobject.h>
#include <imagetransform.h>
#include <gamemap.h>

#include <vector>

class Bomb_attach : public Component
{
public:
    Bomb_attach(int this_strength, bomb_image set_one, Gamemap *this_map, GameObject *come_from, QPoint start_bomb);

private:
    const char *get_image_center(Dir this_dir);
    const char *get_image_bound_full(Dir this_dir);
    const char *get_image_bound_mid(Dir this_dir);
    const char *get_image_bound_half(Dir this_dir);
public:
    void Set_dir_move(Dir set_this);
private:
    void Change_wave_image();
private:
    void Check_and_move(float deltaTime);
    void Bombing_and_over(float deltaTime);
public:
    const QPoint Give_start();
    bool Check_master_equal(GameObject *is_this_master);
public:
    void reset_zero_master();
public:
    //void onAttach() override;
    void onFirstUpdate() override;
    void onUpdate(float deltaTime) override;
    const QPoint Get_start();
    const int Getstrength();
private:
    GameObject *master;
    Gamemap *belong_to;
    std::vector<GameObject *> kill_wave;
    int Edge_and_center[5];
public:
    bool this_kill[player_number];
private:
    const char *bombing_path_normal, *bombing_path_change, *bombing_path_over;
    QPoint Start;
    Bombing_state Now_bomb_state;//用来标记这一张与下一张要显示的图片
    Kill_state Now_kill_state;

private:
    bool bombing;
    int interval;
    int Bomb_strength;

private://最重要的一个成员就是这个时间表示
    float Bombing_left_t;
    float Kill_left_t;

private:
    Dir Moving;
};//在定义中包含这么多的意图就是想将所有的控制交给Control来完成
//之后要做的就是挂载，框架应该（按理）来说应该会直接调用这些参数
#endif  // __BOMB_H_
