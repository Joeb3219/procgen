#ifndef MOB_H_
#define MOB_H_

#include "renderable.h"

class Mob : public FPS_Graphics::Renderable{
public:
    float x, y, z;
    float width, height;
    virtual void render() = 0;
    virtual void update() = 0;
};

class Person : public Mob{
private:
    GLuint texture;
public:
    float x, y, z;
    float width, height;
    Person(float x, float y, float z, GLuint texture);
    void render();
    void update();
};

#endif
