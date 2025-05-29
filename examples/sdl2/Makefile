INCLUDES=-I../../include -I.
CFLAGS=-O2 -std=c99 -Werror -Wall -Wpedantic `sdl2-config --cflags`
CXXFLAGS=-O2 -std=c++11 -Werror -Wall -Wpedantic `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs`

ifneq ($(findstring Linux,$(shell uname -a)),)
	LDFLAGS+=-ldl
endif

%.o: %.c
	gcc $(INCLUDES) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	g++ $(INCLUDES) $(CXXFLAGS) -c $< -o $@

OBJECTS=\
	Audio.o Config.o Entry.o Input.o Logger.o Main.o Perf.o Player.o Video.o \
	../../src/Components.o ../../src/CoreFsm.o ../../src/Frontend.o ../../src/dynlib.o

all: sdl2lrcpp

sdl2lrcpp: $(OBJECTS)
	g++ -o $@ $+ $(LDFLAGS)

clean:
	rm -f sdl2lrcpp $(OBJECTS)

.PHONY: clean
