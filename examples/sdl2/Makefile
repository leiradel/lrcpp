ifeq ($(shell uname -s),)
else ifneq ($(findstring MINGW,$(shell uname -a)),)
    CFLAGS=-mwin32
    CXXFLAGS=-mwin32
else ifneq ($(findstring MSYS,$(shell uname -a)),)
    CFLAGS=-mwin32 -I/mingw64/include
    CXXFLAGS=-mwin32 -I/mingw64/include
    LDFLAGS=-L/mingw64/lib
else ifneq ($(findstring win,$(shell uname -a)),)
endif

INCLUDES+=-I../../include -I.
CFLAGS+=-O2 -std=c99 -Werror -Wall -Wpedantic
CXXFLAGS+=-O2 -std=c++11 -Werror -Wall -Wpedantic
LDFLAGS+=-ldl -lSDL2 -lSDL2main

%.o: %.c
	gcc $(INCLUDES) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	g++ $(INCLUDES) $(CXXFLAGS) -c $< -o $@

OBJECTS=\
	Audio.o Config.o dynlib.o Input.o Logger.o Main.o Perf.o Player.o Video.o \
	../../src/Components.o ../../src/Core.o ../../src/CoreFsm.o ../../src/Frontend.o

all: sdl2lrcpp

sdl2lrcpp: $(OBJECTS)
	g++ -o $@ $+ $(LDFLAGS)

clean:
	rm -f sdl2lrcpp $(OBJECTS)

.PHONY: clean
