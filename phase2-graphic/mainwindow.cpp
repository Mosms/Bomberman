#include <vector>

#include <game_start.h>
#include "./ui_mainwindow.h"
#include <gameobject.h>
#include "mainwindow.h"
#include <imagetransform.h>
#include <transformbuilder.h>

void loadScene(GameScene *gameScene)
{
    //auto map_for_game = new Gamemap();
    //map_for_game->Set_game_scene(gameScene);
    //map_for_game->Init_map();
    auto game_start_ = new Start_game();
    game_start_->Set_game_scene(gameScene);
    game_start_->Let_us_go();
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gameScene = new GameScene();
    gameScene->setParent(this);
    auto view = new QGraphicsView(gameScene, this);
    this->resize(1200, 800);
    view->setFrameStyle(QFrame::NoFrame);
    view->resize(this->size());
    view->setSceneRect(QRect(0, 0, this->width(), this->height()));
    loadScene(gameScene);
}

MainWindow::~MainWindow()
{
    delete ui;
}
