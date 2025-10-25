INCLUDES=-Iinclude
CFLAGS=-O2 -std=c99 -Werror -Wall -Wpedantic
CXXFLAGS=-O2 -std=c++11 -Werror -Wall -Wpedantic

%.o: %.c
	gcc $(INCLUDES) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	g++ $(INCLUDES) $(CXXFLAGS) -c $< -o $@

all: liblrcpp.a

liblrcpp.a: src/Components.o src/CoreFsm.o src/Frontend.o
	ar -crs $@ $+

src/Components.o: src/Components.cpp include/lrcpp/Components.h include/lrcpp/libretro.h

src/CoreFsm.o: src/CoreFsm.cpp include/lrcpp/CoreFsm.h include/lrcpp/libretro.h

src/Frontend.o: src/Frontend.cpp include/lrcpp/Frontend.h include/lrcpp/Components.h include/lrcpp/libretro.h include/lrcpp/CoreFsm.h

clean:
	rm -f liblrcpp.a src/Components.o src/CoreFsm.o src/Frontend.o

.PHONY: clean
