#ifndef HEALTH_H_
#define HEALTH_H_

#include "hitable.h"

class Health : public Hitable
{
public:
    explicit Health(int health);
public:
    bool beHit() override;
    const int How_healthy();
public:
    void One_more_blood();
protected:
    int health;
};

#endif  // HEALTH_H_
