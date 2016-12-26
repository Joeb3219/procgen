#ifndef MOB_H_
#define MOB_H_

#include "renderable.h"

class Mob : public FPS_Graphics::Renderable{
public:
    float x, y, z;
    virtual void render() = 0;
    virtual void update() = 0;
};

class Player : public Mob{
public:
    float x, y, z;
    Player(float x, float y, float z);
    void render();
    void update();
};

#endif
