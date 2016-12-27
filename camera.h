#ifndef CAMERA_H_
#define CAMERA_H_

#include "renderable.h"

class Camera{

private:
    sf::Window *screen;

public:
    float x, y;
    int width, height;

    Camera(int width, int height);
    void render(FPS_Graphics::Renderable& r);
    bool handleEvents();
    void preRender();
    void postRender();
    void windowResized(int width, int height);
    sf::Vector2i getRelativeMousePosition();
    ~Camera();

};

#endif
