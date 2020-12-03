INCLUDES=-Isrc
CFLAGS=-O2 -std=c99 -Werror -Wall -Wpedantic
CXXFLAGS=-O2 -std=c++11 -Werror -Wall -Wpedantic

%.o: %.c
	gcc $(INCLUDES) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	g++ $(INCLUDES) $(CXXFLAGS) -c $< -o $@

all: liblrcpp.a

liblrcpp.a: src/Components.o src/Core.o src/CoreFsm.o src/dynlib.o src/Frontend.o
	ar -crs $@ $+

src/Components.o: src/Components.cpp src/Components.h src/libretro.h

src/Core.o: src/Core.cpp src/Core.h src/libretro.h src/dynlib.h

src/CoreFsm.o: src/CoreFsm.cpp src/CoreFsm.h src/libretro.h src/dynlib.h

src/dynlib.o: src/dynlib.c src/dynlib.h

src/Frontend.o: src/Frontend.cpp src/Frontend.h src/Components.h src/libretro.h src/Core.h src/dynlib.h src/CoreFsm.h

clean:
	rm -f liblrcpp.a src/Components.o src/Core.o src/CoreFsm.o src/dynlib.o src/Frontend.o

.PHONY: clean
