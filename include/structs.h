#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "defs.h"

typedef struct {
    SDL_Rect rect;
    int hardness;
    int bonus;
    int points;
    SDL_Color color;
} Brick;

typedef struct {
    struct {
        int x;
        int y;
    } pos;
    int radius;
    float speed;
    int type;
    SDL_Color color;
    SDL_Rect hitbox;
} Drop;

typedef struct {
    SDL_Rect rect;
    float speed;
    SDL_Color color;
    struct {
        int MOVE;
    } state;
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
    struct {
        int MOVE;
        int MODE;
    } state;
} Ball;

Ball ball;

typedef struct {
    struct {
        Mix_Chunk *br_destroy;
        Mix_Chunk *br_hit;
        Mix_Chunk *wall_hit;
        Mix_Chunk *lost;
        Mix_Chunk *effect;
    } sounds;
} Media;

Media media;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *renderer;
    struct {
        int fps;
        int fps_c;
        long fps_n;
        long prev_time;
        long dt;
        int frc;
    } dev;
    int lives;
    int level;
    int points;
    Brick bricks[MAX_ROWS][MAX_COLS];
    Drop drops[MAX_DROPS];
    struct {
        int QUIT;
        int PAUSE;
        int RUNNING;
    } state;
} App;

App app;

#endif /* !STRUCTS_H_ */
