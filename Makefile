CC := g++ -std=c++11
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lGL -lm
LIB_FATNOISE := lib/fastNoise/FastNoise.cpp
LIB_ALL := $(LIB_FATNOISE)

all: clean fps

fps: main.cpp
	$(CC) -g -o fps $(LIB_ALL) world.cpp math.cpp camera.cpp renderable.cpp main.cpp $(LIBS)

clean:
	rm -rf *.o fps
