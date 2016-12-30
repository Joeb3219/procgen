#include <cstdlib>
#include <ctime>
#include "world.h"
#include "renderable.h"

namespace FPS_World{
        World::World(int size, int seed){
            if(seed == 0){
                srand(time(NULL));
                this->seed = rand();
                srand(1);
            }else this->seed = seed;
            ground = new FPS_Graphics::Ground(size, seed);
        }

        void World::render(){
            ground->render();
        }

        void World::update(){

        }
}
