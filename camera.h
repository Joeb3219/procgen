#ifndef CAMERA_H_
#define CAMERA_H_

class Camera{

private:
    sf::Window *screen;

public:
    float x, y;
    int width, height;

    Camera(int width, int height);
    void render();
    bool handleEvents();
    void preRender();
    void postRender();
    ~Camera();

};

#endif
