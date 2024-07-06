CFLAGS = -Wall -Wextra -pedantic
SFML = -lsfml-graphics -lsfml-window -lsfml-system

all: main

main: main.cpp
	g++ $(CFLAGS) -o main main.cpp $(SFML)

run: main
	./main

rebuild: clean run

clean:
	rm -rf main.o main