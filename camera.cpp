#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "camera.h"
#include "renderable.h"

Camera::Camera(int width, int height){
    x = y = 0.0f;
    this->width = width;
    this->height = height;
    sf::Window *window = new sf::Window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    windowResized(width, height);
    screen = window;
}

sf::Vector2i Camera::getRelativeMousePosition(){
    sf::Vector2i vec = sf::Mouse::getPosition(*screen);
    vec.y = height - vec.y;
    return vec;
}

void Camera::windowResized(int width, int height){
    this->width = width;
    this->height = height;
    std::cout << "Camera::windowResized: " << width << ", " << height << std::endl;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, width, height);
    glOrtho(0, width, 0, height, 0.0f, 1.0f);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Camera::postRender(){
        screen->display();
}

Camera::~Camera(){

}
