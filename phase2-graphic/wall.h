#ifndef _WALL_H_
#define _WALL_H_
//此类为软硬墙的同一个类，一旦我们需要软墙时就只要挂载hittable就可以了
#include <vector>
#include <gameobject.h>

#include <imagetransform.h>
#include <gamescene.h>


class Wall
{
public:
    explicit Wall(QPointF pos);


public:
    bool Itis_hard();
    bool Cannot_go();
    qreal display_zValue();
private:
    void SetImage(const char *path);
    void SetImage(const QString &path);
    void Set_zValue(qreal z_Value);
//---------这里的函数基本都是要直接给gamemap调用的所以基本全部是公有-----//
public:
    void Init_wall_soft(qreal z_Value);
    void Init_wall_hard(qreal z_Value);
public:
    void Get_attached(GameScene *Be_attached);
    void Get_Detached(GameScene *Be_attached);
    void Get_destoryed();
public:
    void En_hitable();
    void Change_image(ImageTransform *change_it_to);
public:
    void Be_hit();

private:
    GameObject *wall;
};


#endif  // _WALL_H_
