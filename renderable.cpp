#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include "renderable.h"
#include "math.h"
#include "noise.h"

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

    Ground::Ground(){
        sf::Vector3f normals [2][256][256];
        sf::Vector3f finalNormals[256 * 256 + 3];
        sf::Vector3f vertices[256 * 256 + 3];
        FPS_Noise::PerlinOctave perlin(8, 12312312);
        double a, b;
        for(a = 0; a < 256; a ++){
            for(b = 0; b < 256; b ++){
                vertices[(int) (a*256 + b)] = sf::Vector3f(a, b, perlin.noise(a, b) * 12);
            }
        }
        for(int x = 0; x < 256; x ++){
            for(int z = 0; z < 256; z ++){
                int ind = x*256 + z;
                sf::Vector3f triangle0[] = {vertices[ind], vertices[ind + 256], vertices[ind + 256 + 1]};
                sf::Vector3f triangle1[] = {vertices[ind + 256 + 1], vertices[ind + 1], vertices[ind]};
                sf::Vector3f tNorm0 = FPS_Math::crossVector(triangle0[0]-triangle0[1], triangle0[1]-triangle0[2]);
                sf::Vector3f tNorm1 = FPS_Math::crossVector(triangle1[0]-triangle1[1], triangle1[1]-triangle1[2]);
                normals[0][x][z] = tNorm0;
                normals[1][x][z] = tNorm1;
            }
        }
        for(int x = 0; x < 256; x ++){
            for(int z = 0; z < 256; z ++){
                sf::Vector3f finalNorm(0, 0, 0);
                if(x != 0 && z != 0) finalNorm += normals[0][x-1][z-1] + normals[1][x-1][z-1];
                if(x != 0 && z != 255) finalNorm += normals[0][x - 1][z];
                if(x != 255 && z != 255) finalNorm += normals[0][x][z] + normals[1][x][z];
                if(x != 255 && z != 0) finalNorm += normals[1][x][z - 1];
                finalNormals[x * 256 + z] = FPS_Math::normalizeVector(finalNorm);
            }
        }

        GLfloat vVerts [256 * 256 * 3];
        GLfloat vNorms [256 * 256 * 3];

        int x, z;
        for(x = 0; x < 256; x ++){
            for(z = 0; z < 256; z ++){
                int ind = x*256 + z;
                vVerts[ind + 0] = vertices[ind].x;
                vVerts[ind + 1] = vertices[ind].y;
                vVerts[ind + 2] = vertices[ind].z;
                vNorms[ind + 0] = finalNormals[ind].x;
                vNorms[ind + 1] = finalNormals[ind].y;
                vNorms[ind + 2] = finalNormals[ind].z;
            }
        }

        glGenBuffers(1, &VBO_VEX_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_VEX_ID);
		glBufferData(GL_ARRAY_BUFFER, 256*256, vVerts, GL_STATIC_DRAW);
        //glVertexAttribPointer()
        glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &VBO_NORM_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_NORM_ID);
		glBufferData(GL_ARRAY_BUFFER, 256*256, vNorms, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    void Ground::render(){
//        glPushMatrix();

//        glEnable(GL_TEXTURE_2D);
//		texture.bind();
		glBindBuffer(GL_ARRAY_BUFFER, VBO_VEX_ID);
		glVertexPointer(3, GL_FLOAT, 0, 0L);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_NORM_ID);
		glNormalPointer(GL_FLOAT, 0, 0L);

		glEnableClientState(GL_VERTEX_ARRAY);
//		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        std::cout << "VEX_ID " << VBO_VEX_ID << ", NORM_ID " << VBO_NORM_ID << std::endl;

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 256*256);

        glDisableClientState(GL_VERTEX_ARRAY);
//		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_TEXTURE_2D);

//        glPopMatrix();
    }

    void Ground::update(){

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
