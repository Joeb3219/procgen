#ifndef RENDERABLE_H_
#define RENDERABLE_H_

class Renderable{
public:
    virtual void render() = 0;
    float x, y;
};

class TestRenderable : public Renderable{
public:
    TestRenderable(float x, float y);
    void render();
    float x, y;
};

#endif
