#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

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

    class Ground : public Renderable{
    private:
        double terrainHeights[256 * 256];
        double terrainNormals[256 * 256 * 3];
        double terrainColors[256 * 256 * 3];
        GLuint DL_ID;
        GLuint generateDL();
        void computeNormals();
        sf::Vector3f terrainCrossProduct(int x1,int z1,int x2,int z2,int x3,int z3);
    public:
        Ground();
        void render();
        void update();
        float x, y;
    };
}

#endif
