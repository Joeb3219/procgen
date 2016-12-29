#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include "lib/fastNoise/FastNoise.h"
#include "renderable.h"
#include "math.h"

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

    GLuint Ground::generateDL(){
        GLuint terrainDL;
    	float startW,startL;
    	int i,j,aux;
        double yOffset = 0, xOffset = 0, zOffset = 0;

    	startW = 0;
    	startL = 0;

    	terrainDL = glGenLists(1);

    	glNewList(terrainDL,GL_COMPILE);

		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

    	// generate n-1 strips, where n = 256
    	// for each vertex test if colors and normals are enabled
    	for (i = 0 ; i < 256-1; i++) {
    		glBegin(GL_TRIANGLE_STRIP);
    		for (j = 0;j < 256; j++) {
    			aux = 3*((i+1)*256 + j);
    			glColor3f(terrainColors[aux], terrainColors[aux+1], terrainColors[aux+2]);
    			glNormal3f(terrainNormals[aux], terrainNormals[aux+1], terrainNormals[aux+2]);
    			glVertex3f(startW + j + xOffset, terrainHeights[(i+1) * 256 + (j)] + yOffset, startL - (i+1) + zOffset);
    			aux = 3 * (i * 256 + j);
    			glColor3f(terrainColors[aux], terrainColors[aux+1], terrainColors[aux+2]);
    			glNormal3f(terrainNormals[aux], terrainNormals[aux+1], terrainNormals[aux+2]);
    			glVertex3f(startW + j + xOffset, terrainHeights[(i) * 256 + j] + yOffset, startL - i + zOffset);
    		}
    		glEnd();
    	}
    	glEndList();

    	// return the list index so that the application can use it
    	return terrainDL;
    }

    Ground::Ground(){
        int goalTerrainMax = 40, goalTerrainMin = 2;

        FastNoise myNoise; // Create a FastNoise object
        myNoise.SetNoiseType(FastNoise::SimplexFractal); // Set the desired noise type
        myNoise.SetFractalOctaves(8);

        double a, b;
        for(a = 0; a < 256; a ++){
            for(b = 0; b < 256; b ++){
                int ind = (int) (a*256 + b), color = 0xFFFFFF;
                double noise = myNoise.GetNoise(a,b);
                terrainHeights[ind] = noise * 20;

                color *= noise;

                terrainColors[ind + 0] = ((color & 0xFF0000) >> 16) / 255.f;
                terrainColors[ind + 1] = ((color & 0x00FF00) >> 8) / 255.f;
                terrainColors[ind + 2] = ((color & 0x0000FF) >> 0) / 255.f;
            }
        }

        computeNormals();
        DL_ID = generateDL();
    }

    sf::Vector3f Ground::terrainCrossProduct(int x1,int z1,int x2,int z2,int x3,int z3) {
        sf::Vector3f v1, v2;

        float terrainStepWidth = 1.0, terrainStepLength = 1.0;

    	v1 = sf::Vector3f((x2-x1) * terrainStepWidth,
            -terrainHeights[z1 * 256 + x1] + terrainHeights[z2 * 256 + x2],
            (z2-z1) * terrainStepLength);

    	v2 = sf::Vector3f((x3-x1) * terrainStepWidth,
            -terrainHeights[z1 * 256 + x1] + terrainHeights[z3 * 256 + x3],
            (z3-z1) * terrainStepLength);

    	return FPS_Math::crossVector(v1, v2);
    }

    void Ground::computeNormals(){
        sf::Vector3f norm1, norm2;
        int i, j, k;

        for(i = 0; i < 256; i++){
    		for(j = 0; j < 256; j++) {

    			if (i == 0 && j == 0) {
    				norm1 = terrainCrossProduct(0,0, 0,1, 1,0);
    				norm1 = FPS_Math::normalizeVector(norm1);
    			}
    			else if (j == 256-1 && i == 256-1) {
    				norm1 = terrainCrossProduct(j,i, j,i-1, j-1,i);
    				norm1 = FPS_Math::normalizeVector(norm1);
    			}
    			else if (j == 0 && i == 256-1) {
    				norm1 = terrainCrossProduct(j,i, j,i-1, j+1,i);
    				norm1 = FPS_Math::normalizeVector(norm1);
    			}
    			else if (j == 256-1 && i == 0) {
    				norm1 = terrainCrossProduct(j,i, j,i+1, j-1,i);
    				norm1 = FPS_Math::normalizeVector(norm1);
    			}

    			/* normals for the borders */
    			else if (i == 0) {
    				norm1 = terrainCrossProduct(j,0, j-1,0, j,1);
    				norm1 = FPS_Math::normalizeVector(norm1);
    				norm2 = terrainCrossProduct(j,0,j,1,j+1,0);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    			}
    			else if (j == 0) {
    				norm1 = terrainCrossProduct(0,i, 1,i, 0,i-1);
    				norm1 = FPS_Math::normalizeVector(norm1);
    				norm2
    					= terrainCrossProduct(0,i, 0,i+1, 1,i);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    			}
    			else if (i == 256-1) {
    				norm1 = terrainCrossProduct(j,i, j,i-1, j+1,i);
    				norm1 = FPS_Math::normalizeVector(norm1);
    				norm2 = terrainCrossProduct(j,i, j+1,i, j,i-1);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    			}
    			else if (j == 256-1) {
    				norm1 = terrainCrossProduct(j,i, j,i-1, j-1,i);
    				norm1 = FPS_Math::normalizeVector(norm1);
    				norm2 = terrainCrossProduct(j,i, j-1,i, j,i+1);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    			}

    			/* normals for the inner vertices using 8 neighbours */
    			else {
    				norm1 = terrainCrossProduct(j,i, j-1,i, j-1,i+1);
    				norm1 = FPS_Math::normalizeVector(norm1);
    				norm2 = terrainCrossProduct(j,i, j-1,i+1, j,i+1);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    				norm2 = terrainCrossProduct(j,i, j,i+1, j+1,i+1);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    				norm2 = terrainCrossProduct(j,i, j+1,i+1, j+1,i);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    				norm2 = terrainCrossProduct(j,i, j+1,i, j+1,i-1);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    				norm2 = terrainCrossProduct(j,i, j+1,i-1, j,i-1);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    				norm2 = terrainCrossProduct(j,i, j,i-1, j-1,i-1);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    				norm2 = terrainCrossProduct(j,i, j-1,i-1, j-1,i);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    			}

    			norm1 = FPS_Math::normalizeVector(norm1);
    			//norm1[2] = -norm1[2];
                terrainNormals[3*(i*256 + j) + 0] = norm1.x;
                terrainNormals[3*(i*256 + j) + 1] = norm1.y;
                terrainNormals[3*(i*256 + j) + 2] = norm1.z;
            }
        }
    }

    void Ground::render(){
        glColor3f(1.f, 1.f, 1.f);
        glCallList(DL_ID);
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
