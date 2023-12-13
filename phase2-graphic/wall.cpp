#include "wall.h"


#include <physics.h>
#include <health.h>
#include <gameobject.h>
#include <imagetransform.h>
#include <props.h>

#include <iostream>

Wall::Wall(QPointF pos)
{
    this->wall = new GameObject();
    auto display_wall = new ImageTransform(pos);
    display_wall->setAlignment(Qt::AlignBottom);
    //此处我们需要保证输入的位置在于像素底端中心，所有下端对齐才是所需效果。
    this->wall->addComponent(display_wall);
    this->wall->addComponent(new Physics);
}
bool Wall::Itis_hard()
{
    return this->wall->getComponent<Hitable>() == nullptr;
}
bool Wall::Cannot_go()
{
    Q_ASSERT(this->wall->getComponent<Physics>() != nullptr);
    return true;
}
qreal Wall::display_zValue()
{
    auto this_transform = this->wall->getComponent<ImageTransform>();
    assert(this_transform != nullptr);
    return this_transform->zValue();
}

void Wall::SetImage(const char *path)
{
    auto transform = this->wall->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    transform->setImage(path);
}
void Wall::SetImage(const QString &path)
{
    auto transform = this->wall->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    transform->setImage(path);
}//just very the same
void Wall::Set_zValue(qreal z_Value)
{
    auto transform = this->wall->getComponent<ImageTransform>();
    Q_ASSERT(transform != nullptr);
    transform->setZValue(z_Value);
}
void Wall::Init_wall_soft(qreal z_Value)
{
    auto getrand = rand() % 8;
    this->Set_zValue(z_Value);
    switch(getrand)
    {
    case 0:
        this->SetImage("../pic-use/soft/a1.png");
        break;
    case 1:
        this->SetImage("../pic-use/soft/a2.png");
        break;
    case 2:
        this->SetImage("../pic-use/soft/b1.png");
        break;
    case 3:
        this->SetImage("../pic-use/soft/b2.png");
        break;
    case 4:
        this->SetImage("../pic-use/soft/b3.png");
        break;
    case 5:
        this->SetImage("../pic-use/soft/i1.png");
        break;
    case 6:
        this->SetImage("../pic-use/soft/m1.png");
        break;
    case 7:
        this->SetImage("../pic-use/soft/m2.png");
        break;
    default:
        Q_ASSERT(6);
        break;
    }
}//继续之前咱的风格，良好的封装性
void Wall::Init_wall_hard(qreal z_Value)
{
    auto getrand = rand() % 2;
    this->Set_zValue(z_Value);
    switch(getrand)
    {
    case 0:
        this->SetImage("../pic-use/hard/m1.png");
        break;
    case 1:
        this->SetImage("../pic-use/hard/m2.png");
        break;
    default:
        Q_ASSERT(2);
        break;
    }
}
void Wall::Get_attached(GameScene *Be_attached)
{
    Be_attached->attachGameObject(this->wall);
}
void Wall::Get_Detached(GameScene *Be_attached)
{
    Be_attached->detachGameObject(this->wall);
}
void Wall::Get_destoryed()
{
    this->wall->destory(this->wall);
}

void Wall::En_hitable()
{
    auto Real_be_hit = new Hitable();
    Q_ASSERT(this->wall != nullptr);
    Q_ASSERT(this->wall->getComponent<Hitable>() == nullptr);
    this->wall->addComponent(Real_be_hit);
}

void Wall::Change_image(ImageTransform *change_it_to)
{
    Q_ASSERT(this->wall->getComponent<ImageTransform>() != nullptr);
    this->wall->removeComponent(this->wall->getComponent<ImageTransform>());
    this->wall->addComponent(change_it_to);
}
void Wall::Be_hit()
{
    auto hit_it = this->wall->getComponent<Hitable>();
    assert(hit_it != nullptr);
    hit_it->beHit();
}
