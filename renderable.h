#ifndef RENDERABLE_H_
#define RENDERABLE_H_

namespace FPS_Graphics{
    class Renderable{
    public:
        virtual void render() = 0;
        virtual void update() = 0;
        float x, y;
    };

    class TestRenderable : public Renderable{
    public:
        TestRenderable(float x, float y);
        void render();
        void update();
        float x, y;
    };
}

#endif
