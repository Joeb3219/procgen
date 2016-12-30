#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include "camera.h"
#include "renderable.h"

#define MOVE_SPEED 2
#define PI 3.14159265
#define NEARCLIP 0.001
#define FARCLIP 400.0
#define FOV 70

Camera::Camera(int width, int height){
    x = y = z = rX = rY = rZ = 0.0f;
    this->width = width;
    this->height = height;
    sf::Window *window = new sf::Window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    windowResized(width, height);
    screen = window;
    mouseGrabbed = focused = true;
    sf::Mouse::setPosition(sf::Vector2i(width / 2, height / 2), *screen);
    glewInit();
}

void Camera::update(){
    if(!focused) return;
    sf::Vector2i mouseChange = getRelativeMousePosition();
    if(mouseGrabbed){
        rX += (mouseChange.x * 0.08);
        rY -= (mouseChange.y * 0.08);
        if(DEBUG == 1) std::cout << "CAMERA: (x, y, z, rX, rY): (" << x << ", " << y << ", " << z << ", " << rX << ", " << rY << std::endl;
        sf::Mouse::setPosition(sf::Vector2i(width / 2, height / 2), *screen);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) mouseGrabbed = !mouseGrabbed;
    float rYRadians = (PI / 180.0) * rY;
    float rXRadians = (PI / 180.0) * (rX + 90);
    float rXAdjustedRadians = (PI / 180.0) * (rX);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        x += (float) cos(rXAdjustedRadians) * MOVE_SPEED;
        z += (float) sin(rXAdjustedRadians) * MOVE_SPEED;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        x -= (float) cos(rXAdjustedRadians) * MOVE_SPEED;
        z -= (float) sin(rXAdjustedRadians) * MOVE_SPEED;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        x -= (float) cos(rXRadians) * MOVE_SPEED * fabs(cos(rYRadians));
        y -= (float) sin(rYRadians) * MOVE_SPEED;
        z -= (float) sin(rXRadians) * MOVE_SPEED * fabs(cos(rYRadians));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        x += (float) cos(rXRadians) * MOVE_SPEED * fabs(cos(rYRadians));
        y += (float) sin(rYRadians) * MOVE_SPEED;
        z += (float) sin(rXRadians) * MOVE_SPEED * fabs(cos(rYRadians));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::U)) y+= MOVE_SPEED;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) y-= MOVE_SPEED;
    if(rX > 360) rX -= 360;
    if(rX < 0) rX += 360;
    if(rY > 90) rY = 90;
    if(rY < -90) rY = -90;

    std::cout << "{x,y,z} " << x << ", " << y << ", " << z << std::endl;
}

sf::Vector2i Camera::getRelativeMousePosition(){
    sf::Vector2i vec = sf::Mouse::getPosition(*screen);
    vec.y = (height / 2) - vec.y;
    vec.x = vec.x - (width / 2);
    return vec;
}

void Camera::windowResized(int width, int height){
    this->width = width;
    this->height = height;
    if(DEBUG == 1) std::cout << "Camera::windowResized: " << width << ", " << height << std::endl;

    glViewport(0, 0, width, height);
    glOrtho(0, width, 0, height, NEARCLIP, FARCLIP);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, (width * 1.0) / height, NEARCLIP, FARCLIP);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
		//glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
		//glCullFace(GL_FRONT_AND_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Camera::gluPerspective(float fov, float aspectRatio, float near, float far){
    GLdouble fW, fH;
    fH = tan(fov / 360 * PI) * near;
    fW = fH * (width * 1.0 / height);
    glFrustum(-fW, fW, -fH, fH, near, far);
}

void Camera::render(FPS_Graphics::Renderable& r){
    r.render();
}

// Handles events of the screen, and then returns true if program should continue executing, or false otherwise.
bool Camera::handleEvents(){
    sf::Event event;
    while (screen->pollEvent(event)){
        if (event.type == sf::Event::Closed) return false;
        else if(event.type == sf::Event::Resized) windowResized(event.size.width, event.size.height);
        else if (event.type == sf::Event::LostFocus) focused = false;
        else if (event.type == sf::Event::GainedFocus) focused = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) return false;
    return true;
}

void Camera::renderHUD(){
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, 128, 0, 128, 0, 1);
    glPopMatrix();
}

void Camera::preRender(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 1.f, 1.f);
    glLoadIdentity();
    glDepthRange(0, 1);
    glMatrixMode(GL_MODELVIEW);
	glRotatef(rY, 1, 0, 0);
	glRotatef(rX, 0, 1, 0);
	glRotatef(rZ, 0, 0, 1);
	glTranslatef(x, y, z);
    glMatrixMode(GL_MODELVIEW);
}

void Camera::postRender(){
        screen->display();
}

Camera::~Camera(){

}
