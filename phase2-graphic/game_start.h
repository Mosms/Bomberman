#ifndef _GAME_START_H_
#define _GAME_START_H_
//此类为软硬墙的同一个类，一旦我们需要软墙时就只要挂载hittable就可以了
#include <vector>
#include <queue>


#include <constant_set.h>
#include <gameobject.h>
#include <imagetransform.h>
#include <utility>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <gamescene.h>


class Start_game
{
public:
    Start_game();
public:
    void Set_game_scene(GameScene *needed_one);
    void Get_attached(GameObject *attach);
    void Get_dettached(GameObject *detach);
public:
    void Init_button();
    void Real_start_();
    void Instruction();
    void Back_from_instru();
    void Set_background();
    void exit_game();
public:
    void Let_us_go();

protected:
    GameScene *gamescene = nullptr;
    QMediaPlayer *Jay_music ;

};

#endif  // _GAME_START_H_
