#include <iostream>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "mob.h"


Player::Player(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

void Player::render(){
    glBegin(GL_POLYGON);
        glColor3f( 0.4f, 0.1f, 0.54f);
        glVertex2f(x + 0.0f, y + 0.0f);
        glVertex2f(x + 32.0f, y + 0.0f);
        glVertex2f(x + 32.0f, y + 64.0f);
        glVertex2f(x + 0.0f, y + 64.0f);
    glEnd();
}

void Player::update(){
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) x--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) x++;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) y--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) y++;
}
