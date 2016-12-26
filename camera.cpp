#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "camera.h"
#include "renderable.h"

Camera::Camera(int width, int height){
    x = y = 0.0f;
    this->width = width;
    this->height = height;
    sf::Window *window = new sf::Window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    glOrtho(0, 1024, 0, 1024, 0.0f, 1.0f);
    screen = window;
}

void Camera::render(Renderable& r){
    r.render();
}

// Handles events of the screen, and then returns true if program should continue executing, or false otherwise.
bool Camera::handleEvents(){
    sf::Event event;
    while (screen->pollEvent(event)){
        if (event.type == sf::Event::Closed) return false;
        else if(event.type == sf::Event::Resized) glViewport(0, 0, event.size.width, event.size.height);
    }
    return true;
}

void Camera::preRender(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera::postRender(){
        screen->display();
}

Camera::~Camera(){

}
