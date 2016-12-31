#ifndef WORLD_H_
#define WORLD_H_

#include <SFML/Window.hpp>
#include "renderable.h"
#include "camera.h"

namespace FPS_World{
    class World : public FPS_Graphics::Renderable{
    private:
        int seed, ticks;
        FPS_Graphics::Ground *ground;
    public:
        World(int size, int seed = 0);
        void update(Camera *camera);
        void render();
        sf::Vector3f getLookingIntersect();
    };
}

#endif
