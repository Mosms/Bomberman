#ifndef _GAME_CONTROL__
#define _GAME_CONTROL__

#include <game_start.h>
#include <constant_set.h>
#include <component.h>
#include <gameobject.h>
#include <gamemap.h>

class control_game : public Component
{
public:
    explicit control_game(Game_control this_type);
public:
    void Set_control_start(Start_game *this_one);
    void Set_control_map(Gamemap *this_one);
public:
    void onClick(QGraphicsSceneMouseEvent *ev) override;
    //void onAttach() override;
    void onFirstUpdate() override;
    void onUpdate(float deltaTime) override;
public:
    void create_new_button();
    void Set_this_type(Game_control this_type);

private:
    Start_game *control_start;
    Gamemap *control_map;
    Game_control this_control;
    GameObject *back_game;
    GameObject *exit_game;
};


#endif//_GAME_CONTROL__
