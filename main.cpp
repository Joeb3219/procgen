#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <sys/time.h>
#include "main.h"
#include "camera.h"
#include "renderable.h"
#include "fileIO.h"
#include "noise.h"
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
    Person person(20, 20, 20, FPS_FileIO::loadBitmapTexture("res/tiles/sheet.bmp"));
    GLuint texture = FPS_FileIO::loadBitmapTexture("res/tiles/test.bmp");
    FPS_Graphics::Tile tile(200, 400, 32, 32, texture);
    FPS_Graphics::Cube cubes [2];
    FPS_Graphics::Ground_Layer ground(10);
    for(int i = 0; i < 2; i ++){
        cubes[i] = FPS_Graphics::Cube(i, 0, 0);
    }

    while (running){
        running = camera->handleEvents();
        camera->preRender();
        camera->render(tile);
        camera->render(person);
        camera->render(ground);
        for(int i = 0; i < 2; i ++) camera->render(cubes[i]);
        camera->postRender();
        while((currentTime + msPerTick) < getCurrentTime()){
            camera->update();
            person.update();
            currentTime += msPerTick;
        }
    }

    return 0;
}
