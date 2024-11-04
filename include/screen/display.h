#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>

#include <SDL2/SDL.h>

#include "ram/bit.h"
#include "arith.h"
#include "hdlc.h"
#include "ram.h"

class display {
    public:
        int height;
        int width;

        bool quit = false;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;

        display(int w, int h);

        void init();
        void poll();
        void read(ram* r, arith* a);
        void end();
};

#endif