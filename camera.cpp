#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include "camera.h"
#include "renderable.h"

#define PI 3.14159265
#define NEARCLIP 0.001
#define FARCLIP 200.0
#define FOV 70


sf::Vector3f normalizeVector(sf::Vector3f vector){
    float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
    return sf::Vector3f(vector.x / length, vector.y / length, vector.z / length);
}

sf::Vector3f crossVector(sf::Vector3f v1, sf::Vector3f v2){
    return sf::Vector3f(
            v1.y*v2.z - v1.z*v2.y,
            v1.z*v2.x - v1.x*v2.z,
            v1.x*v2.y - v1.y*v2.x
    );
}

Camera::Camera(int width, int height){
    x = y = z = rX = rY = 0.0f;
    this->width = width;
    this->height = height;
    sf::Window *window = new sf::Window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    windowResized(width, height);
    screen = window;
    mouseGrabbed = true;
    sf::Mouse::setPosition(sf::Vector2i(width / 2, height / 2), *screen);
}

void Camera::update(){
    sf::Vector2i mouseChange = getRelativeMousePosition();
    if(mouseGrabbed){
        rX += (mouseChange.x * 0.08);
        rY += (mouseChange.y * 0.08);
        std::cout << "CAMERA: (x, y, z, rX, rY): (" << x << ", " << y << ", " << z << ", " << rX << ", " << rY << std::endl;
        sf::Mouse::setPosition(sf::Vector2i(width / 2, height / 2), *screen);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) mouseGrabbed = !mouseGrabbed;
    float rYRadians = (PI / 180.0) * rY;
    float rXRadians = (PI / 180.0) * (rX + 90);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){

    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) x+= 0.01 * cos(rYRadians);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) z-= 0.01;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        x += (float) cos(rXRadians) * 0.01;// * fabs(cos(rYRadians));
        //y -= (float) sin(rXRadians) * 0.01;
        z += (float) sin(rXRadians) * 0.01;// * fabs(cos(rYRadians));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::U)) y+= 0.01;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) y-= 0.01;
    if(rX > 360) rX -= 360;
    if(rX < 0) rX += 360;
    if(rY > 90) rY = 90;
    if(rY < -90) rY = -90;
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
    std::cout << "Camera::windowResized: " << width << ", " << height << std::endl;

    // New things.
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, width, 0, height,NEARCLIP, FARCLIP);
    glLoadIdentity();

    GLdouble fW, fH;
    fH = tan(FOV / 360 * PI) * NEARCLIP;
    fW = fH * (width * 1.0 / height);
    glFrustum(-fW, fW, -fH, fH, NEARCLIP, FARCLIP);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, width, height);
    glOrtho(0, width, 0, height, 0.0f, 1.0f);*/
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
    }
    return true;
}

void Camera::preRender(){
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthRange(0,1);

    float cosRY = cos(rY * (PI / 180.0));
    sf::Vector3f at(x + sin(rX * (PI/180.0)) * FARCLIP * cosRY, y - sin(rY*(PI/180.0)) * FARCLIP, z - cos(rX * (PI/180.0)) * FARCLIP * cosRY);
    sf::Vector3f eye(x, y, z);
    sf::Vector3f up(0, 1, 0);
    sf::Vector3f zAxis = at - eye;
    zAxis = normalizeVector(zAxis);
    sf::Vector3f xAxis = crossVector(up, zAxis);
    xAxis = normalizeVector(xAxis);
    sf::Vector3f yAxis = crossVector(zAxis, xAxis);
    yAxis = normalizeVector(yAxis);

    float matrix[16] = {xAxis.x, yAxis.x, -zAxis.x, 0,
        xAxis.y, yAxis.y, -zAxis.y, 0,
        xAxis.z, yAxis.z, -zAxis.z, 0,
        0, 0, 0, 1};

    /*glRotatef(rY, 1, 0, 0);
    glRotatef(rX, 0, 1, 0);
    glRotatef(0, 0, 0, 1);*/
    glTranslatef(x, y, z);
    glMultMatrixf(matrix);
}

void Camera::postRender(){
        screen->display();
}

Camera::~Camera(){

}
