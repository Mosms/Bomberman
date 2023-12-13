#ifndef HITABLE_H_
#define HITABLE_H_

#include <component.h>

class Hitable : public Component
{
public:
    Hitable();

    virtual bool beHit();
};

#endif  // HITABLE_H_
