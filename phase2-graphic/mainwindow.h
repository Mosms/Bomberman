#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gamescene.h>

#include <gamemap.h>

#include <QGraphicsView>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    GameScene *gameScene;
    Gamemap *land_Map;
};
#endif  // MAINWINDOW_H_
