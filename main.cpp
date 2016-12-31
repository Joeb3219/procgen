#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <sys/time.h>
#include "main.h"
#include "camera.h"
#include "renderable.h"
#include "world.h"

long int getCurrentTime(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int currentTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return currentTime;
}

int main(){
    Camera *camera = new Camera(800, 600);

    double msPerTick = 1000.0 / Settings::tickRate, fps = 0;
    long int currentTime = getCurrentTime();
    long int timeSinceFPSCalculation = currentTime;
    int frames = 0;

    bool running = true;
    FPS_World::World world(512);

    while (running){
        running = camera->handleEvents();
        camera->preRender();
        frames ++;
        camera->render(world);
        camera->postRender();
        while((currentTime + msPerTick) < getCurrentTime()){
            camera->update();
            world.update(camera);
            currentTime += msPerTick;
            std::cout << "FPS: " << fps << std::endl;
        }
        if(frames > 400){
            fps = (frames / ((getCurrentTime() - timeSinceFPSCalculation) / 1000.0));
            timeSinceFPSCalculation = currentTime;
            frames = 0;
        }
    }

    delete camera;

    return 0;
}
