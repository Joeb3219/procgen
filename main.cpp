#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <sys/time.h>
#include "main.h"
#include "camera.h"
#include "renderable.h"
#include "fileIO.h"
#include "mob.h"

long int getCurrentTime(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int currentTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

int main(){
    Camera *camera = new Camera(800, 600);

    double msPerTick = 1000.0 / Settings::tickRate;
    long int currentTime = getCurrentTime();

    bool running = true;
    FPS_Graphics::TestRenderable renderable(0, 900);
    FPS_Graphics::TestRenderable renderable2(100, 300);
    Player player(20, 20, 20);
    GLuint texture = FPS_FileIO::loadBitmapTexture("res/tiles/test.bmp");
    FPS_Graphics::Tile tile(200, 400, 32, 32, texture);
    while (running){
        running = camera->handleEvents();
        camera->preRender();
        camera->render(renderable);
        camera->render(renderable2);
        camera->render(player);
        camera->render(tile);
        camera->postRender();
        while((currentTime + msPerTick) < getCurrentTime()){
            player.update();
            sf::Vector2i mousePos = camera->getRelativeMousePosition();
            currentTime += msPerTick;
        }
    }

    return 0;
}
