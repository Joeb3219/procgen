CC := g++
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lGL

all: clean fps

fps: main.cpp
	$(CC) -g -o fps fileIO.cpp camera.cpp mob.cpp renderable.cpp main.cpp $(LIBS)

clean:
	rm -rf *.o fps
