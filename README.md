"Procedural generation" of terrain in C++

Written with SFML (OpenGL) in C++, this project aims to generate terrain "procedurally" and render it.

Pressing W during the simulation will move you toward where you're facing, S will move away from facing, and A/D will left/right strafe, respectively. Pointing direction is done by mouse movement.

You can release close the simulation by pressing Escape. 

The default terrain (512x512 units) renders at approximately 200FPS on limited laptop hardware (see below). The seed used for rendering is 1337, but this can be changed by changing the following line in Main.cpp:

**FPS_World::World world(512);** to **FPS_World::World world(512, seed);**, where seed is any int. If seed is 0, a random seed will be picked.

The color schemes are a defalt sand-ish and water-ish color depending on elevation, but this can be switched to a black-and-white rendering in code (toggle in renderable.cpp, Ground::generateColorsAndDisplay).

Written in C++, built on Ubuntu Linux 16.04 LTS, on an Intel i5 Processor and Intel Haswell graphics card.

**Installation**

Download or clone the repository, and build via **make all**. 
