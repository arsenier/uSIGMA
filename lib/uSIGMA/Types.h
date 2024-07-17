#pragma once

#include <inttypes.h>

class Tickable
{
public:
    virtual void tick() = 0;
};
