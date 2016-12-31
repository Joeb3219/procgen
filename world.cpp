#include <iostream>
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

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::H) && ticks > 10){
                sf::Vector3f currentPos = camera->getCurrentPosition();

                ticks = 0;
                for(int x = currentPos.x - 3; x <= currentPos.x + 3; x ++){
                    for(int z = currentPos.z - 3; z <= currentPos.z + 3; z ++){
                        ground->modifyHeight(x, z, 1.f);
                    }
                }
                ground->generateColorsAndDisplay();
            }
        }
}
