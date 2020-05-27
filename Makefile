CFLAGS  = -g -O2 -Wall -I./inc -I./
LDFLAGS  = ./Hexapod.o -lpthread -lm -L./ -ldx
# SDL
SDL_LIB = -L/usr/local/lib -lSDL2 -lSDL2_ttf -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include

DXOBJSCXX = $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
DXOBJS = $(patsubst %.c,%.o,$(wildcard src/*.c))

$(DXOBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DXOBJSCXX): %.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

libdx: $(DXOBJS) $(DXOBJSCXX)
	$(AR) rs libdx.a $^

# Compiles Hexapod library
Hexapod.o: Hexapod.cc
	$(CXX) $(CFLAGS) -c $< -o $@
	
Hexapod_SDL.o: Hexapod_SDL.cc Hexapod.o
	$(CXX) $(CFLAGS) $(SDL_INCLUDE) -c $< -o $@ $(SDL_LIB)

manipulation: manipulation.cc Hexapod.o Hexapod_SDL.o
	$(CXX) $(CFLAGS) $(SDL_INCLUDE) manipulation.cc -o manipulation ./Hexapod_SDL.o $(LDFLAGS) $(SDL_LIB)