CC := g++ -std=c++11
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lGL -lm

all: clean fps

fps: main.cpp
	$(CC) -g -o fps noise.cpp fileIO.cpp camera.cpp mob.cpp renderable.cpp main.cpp $(LIBS)

clean:
	rm -rf *.o fps
