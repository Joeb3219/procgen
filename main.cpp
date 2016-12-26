#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "camera.h"
#include "renderable.h"
#include "mob.h"

int main(){
    Camera *camera = new Camera(800, 600);
    //camera->screen->setVerticalSyncEnabled(true);

    bool running = true;
    TestRenderable renderable(0, 900);
    TestRenderable renderable2(100, 300);
    Player player(20, 20, 20);
    while (running){
        running = camera->handleEvents();
        camera->preRender();
        camera->render(renderable);
        camera->render(renderable2);
        camera->render(player);
        camera->postRender();
        player.update();
    }

    return 0;
}
