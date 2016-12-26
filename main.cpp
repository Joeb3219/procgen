#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "camera.h"

int main(){
    Camera *camera = new Camera(800, 600);
    //camera->screen->setVerticalSyncEnabled(true);

    bool running = true;
    while (running){
        running = camera->handleEvents();
        camera->preRender();

        camera->render();
        camera->postRender();
    }

    // release resources...

    return 0;
}
