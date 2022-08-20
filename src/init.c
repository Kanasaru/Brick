#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0) {
        printf("Error initializing SDL_mixer: %s\n", Mix_GetError());
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
    
    app.renderer = SDL_CreateRenderer(app.win, -1, SDL_RENDERER_ACCELERATED);
    
    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
}

void init_game(void)
{
    load_media();

    app.dev.fps = 0;
    app.dev.fps_c = 0;
    app.dev.fps_n = SDL_GetTicks() + 1000;
    app.dev.prev_time = SDL_GetTicks();
    
    app.lives = STARTING_LIVES;
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
    Mix_FreeChunk(media.sounds.br_destroy);
    Mix_FreeChunk(media.sounds.br_hit);
    Mix_FreeChunk(media.sounds.wall_hit);
    media.sounds.br_destroy = NULL;
    media.sounds.br_hit = NULL;
    media.sounds.wall_hit = NULL;

    Mix_Quit();
    TTF_Quit();
    SDL_DestroyWindow(app.win);
    SDL_Quit();
}

void load_media(void)
{
    media.sounds.br_destroy = Mix_LoadWAV(SOUND_BR_DES);
    if (media.sounds.br_destroy == NULL)
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, 
                    "Failed sound: %s\n", Mix_GetError());
    media.sounds.br_hit = Mix_LoadWAV(SOUND_BR_HIT);
    if (media.sounds.br_hit == NULL)
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, 
                    "Failed sound: %s\n", Mix_GetError());
    media.sounds.wall_hit = Mix_LoadWAV(SOUND_WB_HIT);
    if (media.sounds.wall_hit == NULL)
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, 
                    "Failed sound: %s\n", Mix_GetError());
    media.sounds.lost = Mix_LoadWAV(SOUND_LOST);
    if (media.sounds.wall_hit == NULL)
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, 
                    "Failed sound: %s\n", Mix_GetError());
}
