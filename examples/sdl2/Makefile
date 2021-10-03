INCLUDES=-I../../include -I.
CFLAGS=-O2 -std=c99 -Werror -Wall -Wpedantic
CXXFLAGS=-O2 -std=c++11 -Werror -Wall -Wpedantic

%.o: %.c
	gcc $(INCLUDES) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	g++ $(INCLUDES) $(CXXFLAGS) -c $< -o $@

OBJECTS=\
	Config.o dynlib.o Logger.o Video.o \
	../../src/Components.o ../../src/Core.o ../../src/CoreFsm.o ../../src/Frontend.o

all: sdl2lrcpp

sdl2lrcpp: $(OBJECTS)
	g++ -o $@ $+ -lSDL2 -ldl

clean:
	rm -f sdl2lrcpp $(OBJECTS)

.PHONY: clean
