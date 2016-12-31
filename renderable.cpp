#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <ctime>
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

    GLuint Ground::generateDL(){
        GLuint terrainDL;
    	float startW,startL;
    	int i, j, ind;
        double yOffset = 0, xOffset = 0, zOffset = 0;

    	startW = 0;
    	startL = 0;

    	terrainDL = glGenLists(1);

    	glNewList(terrainDL,GL_COMPILE);

    	for (i = 0 ; i < size-1; i++) {
    		glBegin(GL_TRIANGLE_STRIP);
    		for (j = 0;j < size; j++) {
    			ind = 3*((i+1)*size + j);
    			glColor3f(terrainColors[ind], terrainColors[ind+1], terrainColors[ind+2]);
    			glNormal3f(terrainNormals[ind], terrainNormals[ind+1], terrainNormals[ind+2]);
    			glVertex3f(startW + j + xOffset, terrainHeights[(i+1) * size + (j)] + yOffset, startL - (i+1) + zOffset);
    			ind = 3*(i * size + j);
    			glColor3f(terrainColors[ind], terrainColors[ind+1], terrainColors[ind+2]);
    			glNormal3f(terrainNormals[ind], terrainNormals[ind+1], terrainNormals[ind+2]);
    			glVertex3f(startW + j + xOffset, terrainHeights[(i) * size + j] + yOffset, startL - i + zOffset);
    		}
    		glEnd();
    	}
    	glEndList();

    	return terrainDL;
    }

    Ground::~Ground(){
        delete [] terrainHeights;
        delete [] terrainColors;
        delete [] terrainNormals;
    }

    Ground::Ground(int size, int seed){
        this->size = size;
        terrainHeights = new double [size * size];
        terrainColors = new double [3 * size * size];
        terrainNormals = new double [3 * size * size];

        FastNoise myNoise;
        myNoise.SetNoiseType(FastNoise::SimplexFractal);
        myNoise.SetSeed(seed);
        myNoise.SetFractalOctaves(2);

        double a, b;
        for(a = 0; a < size; a ++){
            for(b = 0; b < size; b ++){
                int ind = (int) (a*size + b);
                double noise = FPS_Math::convertScale(myNoise.GetNoise(a,b), -1.0f, 1.f, 0.0f, 1.f);   // Convert noise from b/w -1 -> 1 to 0 -> 1.
                terrainHeights[ind] = noise * 80.0;
            }
        }
        generateColorsAndDisplay();
    }

    void Ground::generateColorsAndDisplay(){
        std::cout << "Regenerating colors and display code" << std::endl;
        int colorMode = 1; // 0 = B&W, 1 = beach.
        int smoothing = 1; // 1 = Smooth, 0 = no smoothing.
        int color = 0xFFFFFF;
        for(int x = 0; x < size; x ++){
            for(int y = 0; y < size; y ++){
                int ind = x * size + y;
                double noise = terrainHeights[ind] / 80.0;
                double r, g, b;
                color = 0xFFFFFF;
                if(colorMode == 0){
                    color *= noise;
                    r = ((color & 0xFF0000) >> 16);// * 0.30;
                    g = ((color & 0x00FF00) >> 8);// * 0.59;
                    b = ((color & 0x0000FF));// * 0.11;

                    color = sqrt((r * r + g * g + b * b) / 3.0);

                    terrainColors[3*ind + 0] = terrainColors[3*ind + 1] = terrainColors[3*ind + 2] = (0xFF - color) / 255.0f;
                }else{
                    color *= noise;

                    if(noise <= 0.5){
                        color = 0x40A4DF;
                        r = ((color & 0xFF0000) >> 16) * 1.0 * (1 - FPS_Math::convertScale(noise, 0.f, 0.5f, 0.f, 0.5f));
                        g = ((color & 0x00FF00) >> 8) * 1.0 * (1 - FPS_Math::convertScale(noise, 0.f, 0.5f, 0.f, 0.5f));
                        b = ((color & 0x0000FF)) * 1.0 * (1 - FPS_Math::convertScale(noise, 0.f, 0.5f, 0.f, 0.5f));
                        color = (((int)r) << 16) + (((int)g) << 8) + ((int)b);
                    }else{
                        color = 0xD3EF6F;
                        r = ((color & 0xFF0000) >> 16) * 1.0 * (1 - FPS_Math::convertScale(noise, 0.5f, 1.f, 0.f, 1.f));
                        g = ((color & 0x00FF00) >> 8) * 1.0 * (1 - FPS_Math::convertScale(noise, 0.5f, 1.f, 0.f, 1.f));
                        b = ((color & 0x0000FF)) * 1.0 * (1 - FPS_Math::convertScale(noise, 0.5f, 1.f, 0.f, 1.f));
                        color = (((int)r) << 16) + (((int)g) << 8) + ((int)b);
                    }

                    r = ((color & 0xFF0000) >> 16);
                    g = ((color & 0x00FF00) >> 8);
                    b = ((color & 0x0000FF));

                    terrainColors[3*ind + 0] = r / 255.0f;
                    terrainColors[3*ind + 1] = g / 255.0f;
                    terrainColors[3*ind + 2] = b / 255.0f;
                }
            }
        }

        if(smoothing){
            for(int x = 1; x < size - 1; x ++){
                for(int z = 1; z < size - 1; z ++ ){
                    int ind = 3*(x * size + z);
                    float lWeight = 1.0, rWeight = 1.0, aWeight = 1.0, bWeight = 1.0, meWeight = 2.0;
                    float weightSums = lWeight + rWeight + aWeight + bWeight + meWeight;
                    float r, g, b;
                    r = terrainColors[ind + 0] *  meWeight +
                        terrainColors[3*((x + 1)*size + z) + 0] * bWeight +
                        terrainColors[3*((x - 1)*size + z) + 0] * aWeight +
                        terrainColors[3*((x)*size + z + 1) + 0] * rWeight +
                        terrainColors[3*((x)*size + z - 1) + 0] * lWeight;
                    g = terrainColors[ind + 1] *  meWeight +
                        terrainColors[3*((x + 1)*size + z) + 1] * bWeight +
                        terrainColors[3*((x - 1)*size + z) + 1] * aWeight +
                        terrainColors[3*((x)*size + z + 1) + 1] * rWeight +
                        terrainColors[3*((x)*size + z - 1) + 1] * lWeight;
                    b = terrainColors[ind + 2] *  meWeight +
                        terrainColors[3*((x + 1)*size + z) + 2] * bWeight +
                        terrainColors[3*((x - 1)*size + z) + 2] * aWeight +
                        terrainColors[3*((x)*size + z + 1) + 2] * rWeight +
                        terrainColors[3*((x)*size + z - 1) + 2] * lWeight;
                        terrainColors[ind + 0] = r / weightSums;
                        terrainColors[ind + 1] = g / weightSums;
                        terrainColors[ind + 2] = b / weightSums;
                }
            }
        }

        computeNormals();
        DL_ID = generateDL();
    }

    double Ground::getHeight(float x, float z){
        int xPrime = (int) x;
        int zPrime = (int) z;
        return terrainHeights[xPrime * size + zPrime];
    }

    void Ground::modifyHeight(float x, float z, float val){
        int xPrime = (int) x;
        int zPrime = (int) z;
        terrainHeights[xPrime * size + zPrime] += val;
        std::cout << "Set height of " << xPrime << ", " << zPrime << " to " << terrainHeights[xPrime * size + zPrime] << std::endl;
    }

    sf::Vector3f Ground::terrainCrossProduct(int x1,int z1,int x2,int z2,int x3,int z3) {
        sf::Vector3f v1, v2;

        float terrainStepWidth = 1.0, terrainStepLength = 1.0;

    	v1 = sf::Vector3f((x2-x1) * terrainStepWidth,
            -terrainHeights[z1 * size + x1] + terrainHeights[z2 * size + x2],
            (z2-z1) * terrainStepLength);

    	v2 = sf::Vector3f((x3-x1) * terrainStepWidth,
            -terrainHeights[z1 * size + x1] + terrainHeights[z3 * size + x3],
            (z3-z1) * terrainStepLength);

    	return FPS_Math::crossVector(v1, v2);
    }

    void Ground::computeNormals(){
        sf::Vector3f norm1, norm2;
        int i, j;

        for(i = 0; i < size; i++){
    		for(j = 0; j < size; j++) {

    			if (i == 0 && j == 0) {
    				norm1 = terrainCrossProduct(0,0, 0,1, 1,0);
    				norm1 = FPS_Math::normalizeVector(norm1);
    			}
    			else if (j == size-1 && i == size-1) {
    				norm1 = terrainCrossProduct(j,i, j,i-1, j-1,i);
    				norm1 = FPS_Math::normalizeVector(norm1);
    			}
    			else if (j == 0 && i == size-1) {
    				norm1 = terrainCrossProduct(j,i, j,i-1, j+1,i);
    				norm1 = FPS_Math::normalizeVector(norm1);
    			}
    			else if (j == size-1 && i == 0) {
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
    			else if (i == size-1) {
    				norm1 = terrainCrossProduct(j,i, j,i-1, j+1,i);
    				norm1 = FPS_Math::normalizeVector(norm1);
    				norm2 = terrainCrossProduct(j,i, j+1,i, j,i-1);
    				norm2 = FPS_Math::normalizeVector(norm2);
    				norm1 += norm2;
    			}
    			else if (j == size-1) {
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
                terrainNormals[3*(i*size + j) + 0] = norm1.x;
                terrainNormals[3*(i*size + j) + 1] = norm1.y;
                terrainNormals[3*(i*size + j) + 2] = norm1.z;
            }
        }
    }

    void Ground::render(){
        glPushMatrix();
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);
        glCallList(DL_ID);
        glDisable(GL_DEPTH_TEST);
        glPopMatrix();
    }

    Cube::Cube(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Cube::Cube(){}

    void Cube::render(){
        float size = 0.5;
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

}
