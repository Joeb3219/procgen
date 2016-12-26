#include <SFML/OpenGL.hpp>
#include "renderable.h"

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
