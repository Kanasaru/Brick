#include <time.h>

#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
#include "event.h"
#include "logic.h"

void calc_fpsdt(void)
{
    /* calculate delta time */
    long now;
    now = SDL_GetTicks();
    app.dev.dt = now - app.dev.prev_time;
    app.dev.prev_time = now;
    
    /* calculate framerate */
    app.dev.fps_c++;
    if (now >= app.dev.fps_n) {
        app.dev.fps = app.dev.fps_c;
        app.dev.fps_c = 0;

        app.dev.fps_n = now + 1000;
    }
}

void run_logic(void)
{
    if (app.state.QUIT == 1) {
        /* do things for quitting */
    }
    
    run_board_logic();
    run_ball_logic();
}

void run_board_logic(void)
{
    if (board.state.MOVE != BOARD_MOVE_NO) {
        int move_x = board.speed * app.dev.dt;
        
        if (board.state.MOVE == BOARD_MOVE_TL) {
            if (board.rect.x - move_x >= 0) {
                board.rect.x -= move_x;
                if (app.state.RUNNING == 0) {
                    ball.pos.x -= move_x;
                    ball.hitbox.x -= move_x;
                }
            }
        }
        if (board.state.MOVE == BOARD_MOVE_TR) {
            if (board.rect.x + move_x + board.rect.w <= SCREEN_WIDTH) {
                board.rect.x += move_x;
                if (app.state.RUNNING == 0) {
                    ball.pos.x += move_x;
                    ball.hitbox.x += move_x;
                }
            }
        }
    }
}

void run_ball_logic(void)
{
    if (ball.state.MOVE != BALL_MOVE_NO) {
        switch (ball.state.MOVE) {
            case BALL_MOVE_TL:
                if (ball.pos.x <= 0) {
                    ball.state.MOVE = 2;
                }
                else if (ball.pos.y - PANEL_HEIGHT <= 0) {
                    ball.state.MOVE = 3;
                }
                else {
                    move_ball(BALL_MOVE_TL);
                }
                break;
            case BALL_MOVE_TR:
                if (ball.pos.x >= SCREEN_WIDTH) {
                    ball.state.MOVE = 1;
                }
                else if (ball.pos.y - PANEL_HEIGHT <= 0) {
                    ball.state.MOVE = 4;
                }
                else {
                    move_ball(BALL_MOVE_TR);
                }
                break;
            case BALL_MOVE_BL:
                if (ball.pos.x <= 0) {
                    ball.state.MOVE = 4;
                }
                else if (ball.pos.y >= SCREEN_HEIGHT) {
                    ball_lost();
                }
                else if (board_collision() == 1) {
                    ball.state.MOVE = 1;
                }
                else {
                    move_ball(BALL_MOVE_BL);
                }
                break;
            case BALL_MOVE_BR:
                if (ball.pos.x >= SCREEN_WIDTH) {
                    ball.state.MOVE = 3;
                }
                else if (ball.pos.y >= SCREEN_HEIGHT) {
                    ball_lost();
                }
                else if (board_collision() == 1) {
                    ball.state.MOVE = 2;
                }
                else {
                    move_ball(BALL_MOVE_BR);
                }
                break;
            case BALL_MOVE_WAIT:
                if (board.state.MOVE == 1) {
                    ball.state.MOVE = 1;
                }
                else if (board.state.MOVE == 2) {
                    ball.state.MOVE = 2;
                }
                else {
                    ball.state.MOVE = (rand() % 2) + 1;
                }
                break;
        }
    }
}

void ball_lost(void)
{
    if (app.lives == 1) {
        /* game over */
        app.lives = 3;
    }
    else {
        app.lives -= 1;
    }
    app.state.RUNNING = 0;
    reset_board();
    reset_ball();
}

void reset_ball(void)
{
    ball.radius = BALL_RADIUS;
    ball.speed = BALL_SPEED;
    ball.color = (SDL_Color) { 255, 255, 255, 255 };
    ball.state.MOVE = BALL_MOVE_NO;
    ball.pos.x = board.rect.x + board.rect.w / 2;
    ball.pos.y = board.rect.y - ball.radius;
    ball.hitbox = (SDL_Rect) { 
        ball.pos.x - ball.radius, 
        ball.pos.y - ball.radius, 
        ball.radius * 2 + 1, 
        ball.radius * 2 + 1 };
}

void reset_board(void)
{
    board.rect = (SDL_Rect) { 
        SCREEN_WIDTH / 2 - BOARD_WIDTH / 2, 
        SCREEN_HEIGHT - BOARD_HEIGHT * 2, 
        BOARD_WIDTH, 
        BOARD_HEIGHT };
    board.state.MOVE = BOARD_MOVE_NO;
    board.speed = BOARD_SPEED;
    board.color = (SDL_Color) { 255, 255, 255, 64 };
}

void move_ball(int direction)
{
    int move_x = ball.speed * app.dev.dt;
    int move_y = ball.speed * app.dev.dt;
    
    switch (direction) {
        case BALL_MOVE_TL:
            ball.pos.x -= move_x;
            ball.pos.y -= move_y;
            ball.hitbox.x -= move_x;
            ball.hitbox.y -= move_y;
            break;
        case BALL_MOVE_TR:
            ball.pos.x += move_x;
            ball.pos.y -= move_y;
            ball.hitbox.x += move_x;
            ball.hitbox.y -= move_y;
            break;
        case BALL_MOVE_BL:
            ball.pos.x -= move_x;
            ball.pos.y += move_y;
            ball.hitbox.x -= move_x;
            ball.hitbox.y += move_y;
            break;
        case BALL_MOVE_BR:
            ball.pos.x += move_x;
            ball.pos.y += move_y;
            ball.hitbox.x += move_x;
            ball.hitbox.y += move_y;
            break;
    }
}

int board_collision(void)
{
    if (ball.hitbox.y + ball.hitbox.h >= board.rect.y) {
        if (ball.pos.x >= board.rect.x) {
            if (ball.pos.x <= board.rect.x + board.rect.w) {
                return 1;
            }
        }
    }
    
    return 0;
}
