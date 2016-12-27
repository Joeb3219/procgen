#include <SFML/OpenGL.hpp>
#include "renderable.h"

namespace FPS_Graphics{
    TestRenderable::TestRenderable(float x, float y){
        this->x = x;
        this->y = y;
    }

    void TestRenderable::render(){
        glBegin(GL_POLYGON);
            glColor3f( 0.2f, 0.3f, 0.12f);
            glVertex2f(x + 0.0f, y + 0.0f);
            glVertex2f(x + 64.0f, y + 0.0f);
            glVertex2f(x + 64.0f, y + 64.0f);
            glVertex2f(x + 0.0f, y + 64.0f);
        glEnd();
    }

    void TestRenderable::update(){

    }

    Tile::Tile(float x, float y, float width, float height, GLuint texture){
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->texture = texture;
    }

    void Tile::render(){
        glEnable(GL_TEXTURE_2D);
        glColor3f( 1.0f, 1.0f, 1.0f);
        glBegin(GL_POLYGON);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexCoord2f(0, 0); glVertex2f(x, y);
            glTexCoord2f(1, 0); glVertex2f(x + width, y);
            glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
            glTexCoord2f(0, 1); glVertex2f(x, y + height);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    void Tile::update(){

    }
}
