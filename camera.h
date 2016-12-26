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
    void render(Renderable& r);
    bool handleEvents();
    void preRender();
    void postRender();
    ~Camera();

};

#endif
