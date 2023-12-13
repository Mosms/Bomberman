#include "health.h"

#include <gameobject.h>
#include <iostream>

Health::Health(int health) : health(health) {}

bool Health::beHit()
{
    health--;
    if (health <= 0)
    {
        return true;
        //std::cout << "be killed" << std::endl;
        //this->gameObject->destory(this->gameObject);
    }
    else return false;
}

const int Health::How_healthy()
{
    return this->health;
}
void Health::One_more_blood()
{
    this->health++;
}
