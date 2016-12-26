#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "camera.h"
#include "renderable.h"

int main(){
    Camera *camera = new Camera(800, 600);
    //camera->screen->setVerticalSyncEnabled(true);

    bool running = true;
    TestRenderable renderable(0, 900);
    TestRenderable renderable2(100, 300);
    while (running){
        running = camera->handleEvents();
        camera->preRender();
        camera->render(renderable);
        camera->render(renderable2);
        camera->postRender();
    }



    return 0;
}
