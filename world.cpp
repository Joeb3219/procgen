#include <cstdlib>
#include <ctime>
#include "world.h"
#include "renderable.h"
#include "camera.h"

namespace FPS_World{
        World::World(int size, int seed){
            if(seed == 0){
                srand(time(NULL));
                this->seed = rand();
                srand(1);
            }else this->seed = seed;
            ground = new FPS_Graphics::Ground(size, seed);
            ticks = 0;
        }

        void World::render(){
            ground->render();
        }

        void World::update(Camera *camera){
            ticks ++;
            sf::Vector3f currentPos = camera->getCurrentPosition();
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::H) && ticks > 10){
                ticks = 0;
                // The world is arbitrarily in the negative quadrant, so we will use positive coords for current system.
                // TODO: Make world render in positive and negative, but have ground cover both regions.
                if(currentPos.x < 0) currentPos.x *= -1;
                if(currentPos.z < 0) currentPos.z *= -1;

                for(int x = currentPos.x - 3; x <= currentPos.x + 3; x ++){
                    for(int z = currentPos.z - 3; z <= currentPos.z + 3; z ++){
                        ground->modifyHeight(x, z, 40.f);
                    }
                }
                ground->generateColorsAndDisplay();
                //ground->modifyHeight(currentPos.x, currentPos.z, 80.f);
            }
        }
}
