#ifndef RENDERABLE_H_
#define RENDERABLE_H_

namespace FPS_Graphics{
    class Renderable{
    public:
        virtual void render() = 0;
        virtual void update() = 0;
        float x, y;
    };

    class Bitmap{

    };

    class Tile : public Renderable{
    private:
        GLuint texture;
        float width, height;
    public:
        float x, y;
        Tile(float x, float y, float width, float height, GLuint texture);
        void render();
        void update();
    };

    class Cube : public Renderable{
    public:
        Cube();
        Cube(float x, float y, float z);
        void render();
        void update();
        float x, y, z;
    };

    class Ground_Layer : public Renderable{
    public:
        Ground_Layer(float width);
        void render();
        void update();
        float width;

    };
}

#endif
