#include <iostream>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "mob.h"


Person::Person(float x, float y, float z, GLuint texture){
    this->texture = texture;
    this->width = 32;
    this->height = 64;
    this->x = x;
    this->y = y;
    this->z = z;
}

void Person::render(){
    glEnable(GL_TEXTURE_2D);
    glColor3f( 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(1, 0); glVertex2f(x + width, y);
        glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
        glTexCoord2f(0, 1); glVertex2f(x, y + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Person::update(){

}
