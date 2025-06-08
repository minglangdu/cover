#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

#include "sdl.h"

using namespace std;

/*
Miscellaneous
*/

bool SDLH::DID_INIT = false;
int SDLH::WINDOWS = 0;

/*
Display
*/

SDLH::Display::Display(string title, int windowsize) {
    if (!SDLH::DID_INIT) {
        SDL_Init(SDL_INIT_EVERYTHING);
        SDLH::DID_INIT = true;
    }
    
    quit = false;
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        windowsize, windowsize, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDLH::WINDOWS ++;

    // check for errors
    if (window == NULL || renderer == NULL) {
        cout << "Error detected: " << SDL_GetError() << "\n";
    }
}

SDLH::Display::~Display() {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    if (SDLH::WINDOWS <= 1) {
        SDL_Quit();
        SDLH::DID_INIT = false;
    }
    SDLH::WINDOWS --;
}

bool SDLH::Display::update() {
    // returns whether quit is true;
    SDL_Event e; 
    while (SDL_PollEvent(&e)) { // seems to be causing the error
        if (e.type == SDL_QUIT) quit = true;
        if (e.window.event == SDL_WINDOWEVENT_CLOSE) quit = true;

        getInputs(e);
    }

    SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0x66, 0xFF);
    SDL_RenderClear(renderer);

    renderObjects();

    SDL_RenderPresent(renderer);
    return quit;
}

void SDLH::Display::getInputs(SDL_Event e) {} // intentionally left blank

void SDLH::Display::renderObjects() {} // intentionally left blank

/*
SoundPlot
*/

SDLH::SoundPlot::SoundPlot(string title, vector<short> graph)
: SDLH::Display::Display(title, 500) {
    vals = graph;
    range.first = vals[0], range.second = vals[0];
    for (short v : vals) {
        range.first = min(range.first, v);
        range.second = max(range.second, v);
    }
    renderObjects();
}

SDLH::SoundPlot::~SoundPlot() {

}

void SDLH::SoundPlot::change(vector<short> graph) {
    vals = graph;
    range.first = vals[0], range.second = vals[0];
    for (short v : vals) {
        range.first = min(range.first, v);
        range.second = max(range.second, v);
    }
}

void SDLH::SoundPlot::renderObjects() {
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(renderer, 0, 250, 500, 250);
    double prev = 0; double x = 0;
    for (int i = 0; i < 5000; i ++) {
        short v = vals[i];
        double next = ((v - 2 * range.first) / (range.first + range.second));
        SDL_RenderDrawLine(renderer, (int)x, (int)prev, (int)(x + 500 / vals.size()), (int)next);
        // x += 500 / vals.size();
        x += 0.1;
        prev = next;
        // cout << x << " " << prev << "\n";
    }
    SDL_RenderPresent(renderer);
}

bool SDLH::SoundPlot::update() {
    // returns whether quit is true;
    SDL_Event e; 
    while (SDL_PollEvent(&e)) { // seems to be causing the error
        if (e.type == SDL_QUIT) quit = true;
        if (e.window.event == SDL_WINDOWEVENT_CLOSE) quit = true;

        getInputs(e);
    }

    // SDL_RenderPresent(renderer);
    return quit;
}