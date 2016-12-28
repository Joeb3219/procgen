#ifndef CAMERA_H_
#define CAMERA_H_

#include "renderable.h"

class Camera{

private:
    sf::Window *screen;
    float x, y, z;
    float rX, rY, rZ;
    bool mouseGrabbed;
    void gluPerspective(float fov, float aspectRatio, float near, float far);

public:
    int width, height;

    Camera(int width, int height);
    void render(FPS_Graphics::Renderable& r);
    bool handleEvents();
    void preRender();
    void postRender();
    void windowResized(int width, int height);
    void update();
    sf::Vector2i getRelativeMousePosition();
    ~Camera();

};

#endif
