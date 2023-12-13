#ifndef _PROPS_H_
#define _PROPS_H_

//此类道具类，实现想法是将其设置为如墙类一般的游戏对象指针类，本质还是要实现一个特殊的游戏对象

#include <constant_set.h>
#include <gameobject.h>

#include <imagetransform.h>
#include <gamescene.h>


class Props
{
public:
    Props(Item_check this_prop_type, QPointF this_pos, qreal Z_value, GameScene *belong_to);

private:
    void SetImage(const char *path);
    void SetImage(const QString &path);
    void Set_zValue(qreal z_Value);
private:
    void Init_image();
public:
    void Be_gotton(GameObject *Gainer);

private:
    Item_check its_type;
    GameObject *prop;
};


#endif  // _WALL_H_
