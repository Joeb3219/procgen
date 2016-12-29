#include <SFML/OpenGL.hpp>
#include "renderable.h"

namespace FPS_Graphics{

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
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(x, y);
            glTexCoord2f(1, 0); glVertex2f(x + width, y);
            glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
            glTexCoord2f(0, 1); glVertex2f(x, y + height);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    void Tile::update(){

    }

    Ground_Layer::Ground_Layer(float width){
        this->width = width;
    }

    void Ground_Layer::render(){
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(-width/2.0, 0, -width/2.0);
        glRotatef(90, 1, 0, 0);
        glColor3f(0xC2 / 255.0f, 0xB2 / 255.0f, 0x80 / 255.0f); // #c2b280
        glBegin(GL_QUADS);
            glVertex2f(0, 0);
            glVertex2f(width / 2.0, 0);
            glVertex2f(width / 2.0, width / 2.0);
            glVertex2f(0, width / 2.0);
        glEnd();
        glPopMatrix();
    }

    void Ground_Layer::update(){

    }

    Cube::Cube(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Cube::Cube(){}

    void Cube::render(){
        float size = 0.5;
        //glLoadIdentity();
        glPushMatrix();

        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
        glTranslatef(x, y, z);
  glBegin(GL_QUADS);        // Draw The Cube Using quads
    glColor3f(0.0f,1.0f,0.0f);    // Color Blue
    glVertex3f( size, size,-size);    // Top Right Of The Quad (Top)
    glVertex3f(-size, size,-size);    // Top Left Of The Quad (Top)
    glVertex3f(-size, size, size);    // Bottom Left Of The Quad (Top)
    glVertex3f( size, size, size);    // Bottom Right Of The Quad (Top)
    glColor3f(1.0f,0.5f,0.0f);    // Color Orange
    glVertex3f( size,-size, size);    // Top Right Of The Quad (Bottom)
    glVertex3f(-size,-size, size);    // Top Left Of The Quad (Bottom)
    glVertex3f(-size,-size,-size);    // Bottom Left Of The Quad (Bottom)
    glVertex3f( size,-size,-size);    // Bottom Right Of The Quad (Bottom)
    glColor3f(1.0f,0.0f,0.0f);    // Color Red
    glVertex3f( size, size, size);    // Top Right Of The Quad (Front)
    glVertex3f(-size, size, size);    // Top Left Of The Quad (Front)
    glVertex3f(-size,-size, size);    // Bottom Left Of The Quad (Front)
    glVertex3f( size,-size, size);    // Bottom Right Of The Quad (Front)
    glColor3f(1.0f,1.0f,0.0f);    // Color Yellow
    glVertex3f( size,-size,-size);    // Top Right Of The Quad (Back)
    glVertex3f(-size,-size,-size);    // Top Left Of The Quad (Back)
    glVertex3f(-size, size,-size);    // Bottom Left Of The Quad (Back)
    glVertex3f( size, size,-size);    // Bottom Right Of The Quad (Back)
    glColor3f(0.0f,0.0f,1.0f);    // Color Blue
    glVertex3f(-size, size, size);    // Top Right Of The Quad (Left)
    glVertex3f(-size, size,-size);    // Top Left Of The Quad (Left)
    glVertex3f(-size,-size,-size);    // Bottom Left Of The Quad (Left)
    glVertex3f(-size,-size, size);    // Bottom Right Of The Quad (Left)
    glColor3f(1.0f,0.0f,1.0f);    // Color Violet
    glVertex3f( size, size,-size);    // Top Right Of The Quad (Right)
    glVertex3f( size, size, size);    // Top Left Of The Quad (Right)
    glVertex3f( size,-size, size);    // Bottom Left Of The Quad (Right)
    glVertex3f( size,-size,-size);    // Bottom Right Of The Quad (Right)
  glEnd();            // End Drawing The Cube
  glDisable(GL_DEPTH_TEST);
  glPopMatrix();
    }

    void Cube::update(){

    }
}
