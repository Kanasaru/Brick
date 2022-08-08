#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <SDL2/SDL.h>

#include "defs.h"

typedef struct {
    SDL_Rect rect;
    int hardness;
    float bonus;
    SDL_Color color;
} Brick;

typedef struct {
    SDL_Rect rect;
    int speed;
    SDL_Color color;
} Board;

Board board;

typedef struct {
    struct {
        int x;
        int y;
    } pos;
    int radius;
    float speed;
    float damage;
    SDL_Color color;
    SDL_Rect hitbox;
} Ball;

Ball ball;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *renderer;
    struct {
        int fps;
        int fps_c;
        long fps_n;
        float lgcrt;
    } dev;
    int level;
    int points;
    Brick bricks[MAX_ROWS][MAX_COLS];
    struct {
        int START_GAME;
        int QUIT_GAME;
        int PAUSE_GAME;
        int KEY_LEFT;
        int KEY_RIGHT;
        int KEY_SPACE;
    } events;
    struct {
        int QUIT;
        int PAUSE;
        int RUNNING;
    } state;
} App;

App app;

#endif /* !STRUCTS_H_ */
