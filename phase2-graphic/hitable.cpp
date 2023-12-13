#include "hitable.h"

Hitable::Hitable() {}

bool Hitable::beHit()
{
    destory(this->gameObject);
    return true;
}
