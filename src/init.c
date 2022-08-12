#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "defs.h"
#include "structs.h"
#include "init.h"
#include "level.h"
#include "brick.h"
#include "logic.h"

void init_SDL(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    if (TTF_Init() < 0) {
        printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    
    app.win = SDL_CreateWindow(
        GAME_TITLE, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        0
    );
    
    if (app.win == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* app.renderer = SDL_CreateRenderer(app.win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED); */
    app.renderer = SDL_CreateRenderer(app.win, -1, SDL_RENDERER_ACCELERATED);
    
    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void init_game(void)
{
    app.dev.fps = 0;
    app.dev.fps_c = 0;
    app.dev.fps_n = SDL_GetTicks() + 1000;
    app.dev.prev_time = SDL_GetTicks();
    app.dev.dt = 0;
    
    app.lives = 3;
    app.level = 1;
    app.points = 0;
    
    app.state.QUIT = 0;
    app.state.PAUSE = 0;
    app.state.RUNNING = 0;
    
    load_bricks(LEVEL_01);
    
    reset_board();
    reset_ball();
}

void clr_init(void)
{
    TTF_Quit();
    SDL_DestroyWindow(app.win);
    SDL_Quit();
}
