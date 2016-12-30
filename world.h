#ifndef WORLD_H_
#define WORLD_H_

#include "renderable.h"

namespace FPS_World{
    class World : public FPS_Graphics::Renderable{
    private:
        int seed;
        FPS_Graphics::Ground *ground;
    public:
        World(int size, int seed = 0);
        void update();
        void render();
    };
}

#endif
