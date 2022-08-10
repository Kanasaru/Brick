#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
#include "event.h"
#include "logic.h"

void calc_fps(void)
{
    app.dev.fps_c++;

    if (SDL_GetTicks() >= app.dev.fps_n) {
        app.dev.fps = app.dev.fps_c;
        app.dev.fps_c = 0;

        app.dev.fps_n = SDL_GetTicks() + 1000;
    }
    
    app.dev.lgcrt = calc_lgcrt(app.dev.fps);
}

float calc_lgcrt(int fps)
{
    float factor;
    
    /* needs improvement */
    factor = (float) FPS / fps;
    /* strip decimal places to 2
     * factor = (int) (factor * 100);
     * factor = factor / 100;
     */
    
    return factor;
}

void run_logic(void)
{
    calc_fps();
    
    
    if (app.state.QUIT == 1) {
        /* do things for quitting */
    }
    
    
    run_board_logic();
    run_ball_logic();
}

void run_board_logic(void)
{
    if (board.state.MOVE != 0) {
        int move_x = board.speed;
        
        if (board.state.MOVE == 1) {
            if (board.rect.x - move_x >= 0) {
                board.rect.x -= move_x;
                if (app.state.RUNNING == 0) {
                    ball.pos.x -= move_x;
                }
            }
        }
        if (board.state.MOVE == 2) {
            if (board.rect.x + move_x + board.rect.w <= SCREEN_WIDTH) {
                board.rect.x += move_x;
                if (app.state.RUNNING == 0) {
                    ball.pos.x += move_x;
                }
            }
        }
    }
}

void run_ball_logic(void)
{
    if (ball.state.MOVE != 0) {
        int move_x = ball.speed;
        int move_y = ball.speed;
        
        switch (ball.state.MOVE) {
            case 1:
                if (ball.pos.x - move_x <= 0) {
                    ball.state.MOVE = 2;
                }
                else if (ball.pos.y - move_y <= 0) {
                    ball.state.MOVE = 3;
                }
                else {
                    ball.pos.x -= move_x;
                    ball.pos.y -= move_y;
                }
                break;
            case 2:
                if (ball.pos.x + move_x >= SCREEN_WIDTH) {
                    ball.state.MOVE = 1;
                }
                else if (ball.pos.y - move_y <= 0) {
                    ball.state.MOVE = 4;
                }
                else {
                    ball.pos.x += move_x;
                    ball.pos.y -= move_y;
                }
                break;
            case 3:
                if (ball.pos.x - move_x <= 0) {
                    ball.state.MOVE = 4;
                }
                else if (ball.pos.y + move_y >= SCREEN_HEIGHT) {
                    reset_ball();
                }
                else if (board_collision(ball.pos.x - move_x, ball.pos.y + move_y) == 1) {
                    ball.state.MOVE = 1;
                }
                else {
                    ball.pos.x -= move_x;
                    ball.pos.y += move_y;
                }
                break;
            case 4:
                if (ball.pos.x + move_x >= SCREEN_WIDTH) {
                    ball.state.MOVE = 3;
                }
                else if (ball.pos.y + move_y >= SCREEN_HEIGHT) {
                    reset_ball();
                }
                else if (board_collision(ball.pos.x + move_x, ball.pos.y + move_y) == 1) {
                    ball.state.MOVE = 2;
                }
                else {
                    ball.pos.x += move_x;
                    ball.pos.y += move_y;
                }
                break;
            case 5:
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

void reset_ball(void)
{
    ball.state.MOVE = 0;
    app.state.RUNNING = 0;
    ball.pos.x = board.rect.x + board.rect.w / 2;
    ball.pos.y = board.rect.y - ball.radius;
}

int board_collision(int x, int y)
{
    if (y >= board.rect.y) {
        if (x >= board.rect.x) {
            if (x <= board.rect.x + board.rect.w) {
                return 1;
            }
        }
    }
    
    return 0;
}
