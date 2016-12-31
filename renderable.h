#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

namespace FPS_Graphics{

    class Renderable{
    public:
        virtual void render() = 0;
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
    };

    class Cube : public Renderable{
    public:
        Cube();
        Cube(float x, float y, float z);
        void render();
        float x, y, z;
    };

    class Ground : public Renderable{
    private:
        double *terrainHeights;
        double *terrainNormals;
        double *terrainColors;
        GLuint DL_ID;
        GLuint generateDL();
        int size;
        void computeNormals();
        sf::Vector3f terrainCrossProduct(int x1,int z1,int x2,int z2,int x3,int z3);
    public:
        Ground(int size, int seed);
        double getHeight(float x, float z);
        void modifyHeight(float x, float z, float val);
        void generateColorsAndDisplay();
        ~Ground();
        void render();
        float x, y;
    };
}

#endif
