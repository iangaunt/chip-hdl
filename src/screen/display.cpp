#include "screen/display.h"

#include <iostream>

#include <SDL2/SDL.h>

#include "ram.h"

const signed int ON = 0xFFFFFFFF;
const signed int OFF = 0xFF000000;

/**
 * Creates a new `display` object.
 * 
 * @param w The width of the window.
 * @param h The height of the window.
 */
display::display(int w, int h) {
    height = h;
    width = w;
    quit = false;
}

/**
 * Initializes the display object by generating a new window, renderer,
 * and texture object for showing graphics from the memory buffer.
 */
void display::init() {
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        "display", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        width, height, 
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, width, height);
    
    texture = texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        width, height
    );

    if (window == NULL) return;
}

/** Polls any events that occur on the window. */
void display::poll() {
    SDL_Event event;
    if (quit == true) end();

    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                quit = true;
                break;
            }  
        
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: { 
                        quit = true; 
                        break;
                    }
                    
                    default: { 
                        break; 
                    }
                }

                break;
            } 
        }
    }
}

/**
 * Reads in the values of the ram buffer starting from index `24576` and then
 * reads pixel by pixel into the video buffer. `true` values will turn
 * white and `false` pixels will turn black.
 * 
 * @param r The `ram` chip to read from.
 * @param a The `alu` chip used for incrementing the pointer index of the memory buffer.
 */
void display::read(ram* r, alu* a) {
    bool* index = new bool[16];
    for (int i = 0; i < 16; i++) {
        index[i] = false;
    }
    index[1] = true;
    index[2] = true;
    
    int* pixels = new int[width * height];

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (j > 0 && j % 16 == 0) index = a->INC16(index);
            pixels[i * height + j] = r->GET(index)[j % 16] ? ON : OFF;
        }
    }

    SDL_UpdateTexture(texture, nullptr, pixels, 512 * sizeof(unsigned int));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

/** Destroys the graphics program. */
void display::end() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}