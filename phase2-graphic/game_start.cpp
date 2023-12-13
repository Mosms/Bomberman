#include "game_start.h"


#include "qurl.h"
#include <gamemap.h>
#include <game_control.h>

#include <QApplication>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <iostream>
Start_game::Start_game() {}
void Start_game::Set_game_scene(GameScene *needed_one)
{
    this->gamescene = needed_one;
}
void Start_game::Get_attached(GameObject * attach)
{
    this->gamescene->attachGameObject(attach);
}
void Start_game::Get_dettached(GameObject * detach)
{
    this->gamescene->detachGameObject(detach);
}
void Start_game::Set_background()
{
    auto back_ground = new GameObject();
    back_ground->addComponent(new ImageTransform(this->gamescene->sceneRect().topLeft()));
    back_ground->getComponent<ImageTransform>()->setImage("../pic-use/others/back_ground.png");//back_1.png
    back_ground->getComponent<ImageTransform>()->setZValue(-1);
    this->Get_attached(back_ground);
    if(Jay_music == nullptr)
    {
        auto player = new QMediaPlayer;
        auto audioOutput = new QAudioOutput;
        player->setAudioOutput(audioOutput);
        player->setSource(QUrl::fromLocalFile("../music/background.wav"));
        audioOutput->setVolume(20);
        this->Jay_music = player;
        this->Jay_music->play();
    }
    Q_ASSERT(this->Jay_music != nullptr);
}
void Start_game::Init_button()
{
    //Q_ASSERT(this->gamescene->Scene_empty());//初始时应为空
    auto Game_start = new GameObject();
    QPointF stratbutton_pos(200, 200);
    Game_start->addComponent(new ImageTransform(this->gamescene->sceneRect().topLeft() + stratbutton_pos));
    Game_start->addComponent(new control_game(start));
    Game_start->getComponent<control_game>()->Set_control_start(this);
    this->Get_attached(Game_start);
    auto instru_ment = new GameObject();
    QPointF instru_pos(200, 350);
    instru_ment->addComponent(new ImageTransform(this->gamescene->sceneRect().topLeft() + instru_pos));
    instru_ment->addComponent(new control_game(instru));
    instru_ment->getComponent<control_game>()->Set_control_start(this);
    this->Get_attached(instru_ment);
    auto out_going = new GameObject();
    QPointF out_pos(200, 500);
    out_going->addComponent(new ImageTransform(this->gamescene->sceneRect().topLeft() + out_pos));
    out_going->addComponent(new control_game(out));
    out_going->getComponent<control_game>()->Set_control_start(this);
    this->Get_attached(out_going);
}
void Start_game::Real_start_()
{
    this->gamescene->Clear_it();
    auto map_for_game = new Gamemap();
    map_for_game->Set_game_scene(gamescene);
    map_for_game->Init_map();
    map_for_game->Setmusic(this->Jay_music);
}
void Start_game::Instruction()
{
    this->gamescene->Clear_it();
    this->Set_background();
    QPointF Rule_pos(100, 100);
    auto rule = new GameObject();
    rule->addComponent(new ImageTransform(this->gamescene->sceneRect().topLeft() + Rule_pos));
    rule->getComponent<ImageTransform>()->setImage("../pic-use/others/rule.png");
    this->Get_attached(rule);
    QPointF back_pos(800, 600);
    auto reack_this = new GameObject();
    reack_this->addComponent(new ImageTransform(this->gamescene->sceneRect().topLeft() + back_pos));
    reack_this->addComponent(new control_game(reack));
    reack_this->getComponent<control_game>()->Set_control_start(this);
    this->Get_attached(reack_this);
}
void Start_game::Back_from_instru()
{
    this->gamescene->Clear_it();
    this->Let_us_go();//完全一样的操作，除了重复设置了一下背景（花不了多少时间）
}

void Start_game::Let_us_go()
{
    this->Set_background();
    this->Init_button();
}
void Start_game::exit_game()
{
    QApplication::quit();
}
