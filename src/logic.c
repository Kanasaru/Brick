#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
#include "event.h"
#include "logic.h"

void fps_dt(void)
{
    long now;
    now = SDL_GetTicks();
    app.dev.dt = now - app.dev.prev_time;
    app.dev.prev_time = now;
    
    app.dev.fps_c++;
    if (now >= app.dev.fps_n) {
        app.dev.fps = app.dev.fps_c;
        app.dev.fps_c = 0;

        app.dev.fps_n = now + 1000;
    }
    
    app.dev.frc = floor((1000 + app.dev.dt) / FPS);
    if (app.dev.frc < 0 )
        app.dev.frc = 0;
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
        int brick_hit = brick_collision();
        int board_hit = board_collision();
        
        switch (ball.state.MOVE) {
            case BALL_MOVE_TL:
                if (ball.pos.x <= 0) {
                    ball.state.MOVE = BALL_MOVE_TR;
                }
                if (ball.pos.y - PANEL_HEIGHT <= 0) {
                    ball.state.MOVE = BALL_MOVE_BL;
                }
                if (brick_hit != BALL_HIT_NONE) {
                    switch(brick_hit) {
                        case BALL_HIT_LEFT:
                            ball.state.MOVE = BALL_MOVE_TR;
                            break;
                        case BALL_HIT_TOP:
                            ball.state.MOVE = BALL_MOVE_BL;
                            break;
                        case BALL_HIT_TL:
                            ball.state.MOVE = BALL_MOVE_BR;
                            break;
                    }
                }
                move_ball(BALL_MOVE_TL);
                break;
            case BALL_MOVE_TR:
                if (ball.pos.x >= SCREEN_WIDTH) {
                    ball.state.MOVE = BALL_MOVE_TL;
                }
                if (ball.pos.y - PANEL_HEIGHT <= 0) {
                    ball.state.MOVE = BALL_MOVE_BR;
                }
                if (brick_hit != BALL_HIT_NONE) {
                    switch(brick_hit) {
                        case BALL_HIT_RIGHT:
                            ball.state.MOVE = BALL_MOVE_TL;
                            break;
                        case BALL_HIT_TOP:
                            ball.state.MOVE = BALL_MOVE_BR;
                            break;
                        case BALL_HIT_TR:
                            ball.state.MOVE = BALL_MOVE_BL;
                            break;
                    }
                }
                move_ball(BALL_MOVE_TR);
                break;
            case BALL_MOVE_BL:
                if (ball.pos.x <= 0) {
                    ball.state.MOVE = BALL_MOVE_BR;
                }
                if (ball.pos.y >= SCREEN_HEIGHT) {
                    ball_lost();
                    break;
                }
                if (board_hit != BALL_HIT_NONE) {
                    switch(board_hit) {
                        case BALL_HIT_BOTTOM:
                            ball.state.MOVE = BALL_MOVE_TL;
                            break;
                        case BALL_HIT_BL:
                            ball.state.MOVE = BALL_MOVE_TR;
                            break;
                    }
                }
                if (brick_hit != BALL_HIT_NONE) {
                    switch(brick_hit) {
                        case BALL_HIT_LEFT:
                            ball.state.MOVE = BALL_MOVE_BR;
                            break;
                        case BALL_HIT_BOTTOM:
                            ball.state.MOVE = BALL_MOVE_TL;
                            break;
                        case BALL_HIT_BL:
                            ball.state.MOVE = BALL_MOVE_TR;
                            break;
                    }
                }
                move_ball(BALL_MOVE_BL);
                break;
            case BALL_MOVE_BR:
                if (ball.pos.x >= SCREEN_WIDTH) {
                    ball.state.MOVE = BALL_MOVE_BL;
                }
                if (ball.pos.y >= SCREEN_HEIGHT) {
                    ball_lost();
                    break;
                }
                if (board_hit != BALL_HIT_NONE) {
                    switch(board_hit) {
                        case BALL_HIT_BOTTOM:
                            ball.state.MOVE = BALL_MOVE_TR;
                            break;
                        case BALL_HIT_BR:
                            ball.state.MOVE = BALL_MOVE_TL;
                            break;
                    }
                }
                if (brick_hit != BALL_HIT_NONE) {
                    switch(brick_hit) {
                        case 1:
                            break;
                        case BALL_HIT_RIGHT:
                            ball.state.MOVE = BALL_MOVE_BL;
                            break;
                        case BALL_HIT_BOTTOM:
                            ball.state.MOVE = BALL_MOVE_TR;
                            break;
                        case BALL_HIT_BR:
                            ball.state.MOVE = BALL_MOVE_TL;
                            break;
                    }
                }
                move_ball(BALL_MOVE_BR);
                break;
            case BALL_MOVE_WAIT:
                if (board.state.MOVE == BOARD_MOVE_TL) {
                    ball.state.MOVE = BALL_MOVE_TL;
                }
                else if (board.state.MOVE == BOARD_MOVE_TR) {
                    ball.state.MOVE = BALL_MOVE_TR;
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
        app.lives = STARTING_LIVES;
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
    
    SDL_Rect ball_b = { ball.hitbox.x + ball.hitbox.w / 2, 
                               ball.hitbox.y + ball.hitbox.h, 1, 1 };
    SDL_Rect ball_bl = { ball.hitbox.x, 
                             ball.hitbox.y + ball.hitbox.h, 1, 1 };
    SDL_Rect ball_br = { ball.hitbox.x + ball.hitbox.w, 
                             ball.hitbox.y + ball.hitbox.h, 1, 1 };
    SDL_Rect res_rect;
    
    if (SDL_IntersectRect(&board.rect, &ball.hitbox, &res_rect) == SDL_TRUE) {
        if (SDL_IntersectRect(&board.rect, &ball_b, &res_rect) == SDL_TRUE) {
            return BALL_HIT_BOTTOM;
        }
        if (SDL_IntersectRect(&board.rect, &ball_bl, &res_rect) == SDL_TRUE) {
            return BALL_HIT_BL;
        }
        if (SDL_IntersectRect(&board.rect, &ball_br, &res_rect) == SDL_TRUE) {
            return BALL_HIT_BR;
        }
    }
    return 0;
}

int brick_collision(void)
{
    SDL_Rect ball_t = { ball.hitbox.x + ball.hitbox.w / 2, 
                            ball.hitbox.y, 1, 1 };
    SDL_Rect ball_b = { ball.hitbox.x + ball.hitbox.w / 2, 
                               ball.hitbox.y + ball.hitbox.h, 1, 1 };
    SDL_Rect ball_l = { ball.hitbox.x, 
                             ball.hitbox.y + ball.hitbox.h / 2, 1, 1 };
    SDL_Rect ball_r = { ball.hitbox.x + ball.hitbox.w, 
                              ball.hitbox.y + ball.hitbox.h / 2, 1, 1 };
    
    SDL_Rect ball_tl = { ball.hitbox.x + BALL_CORNER_AC, 
                             ball.hitbox.y + BALL_CORNER_AC, 1, 1 };
    SDL_Rect ball_tr = { ball.hitbox.x + ball.hitbox.w - BALL_CORNER_AC, 
                             ball.hitbox.y + BALL_CORNER_AC, 1, 1 };
    SDL_Rect ball_bl = { ball.hitbox.x + BALL_CORNER_AC, 
                             ball.hitbox.y + ball.hitbox.h - BALL_CORNER_AC, 
                             1, 1 };
    SDL_Rect ball_br = { ball.hitbox.x + ball.hitbox.w - BALL_CORNER_AC, 
                             ball.hitbox.y + ball.hitbox.h - BALL_CORNER_AC, 
                             1, 1 };
    SDL_Rect res;

    for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_COLS; ++j) {
            if (app.bricks[i][j].hardness > 0) {
                if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                      &ball.hitbox, &res) == SDL_TRUE) {
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                          &ball_l, &res) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return BALL_HIT_LEFT;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                          &ball_r, &res) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return BALL_HIT_RIGHT;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                          &ball_t, &res) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return BALL_HIT_TOP;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                          &ball_b, &res) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return BALL_HIT_BOTTOM;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                          &ball_tl, &res) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return BALL_HIT_TL;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                          &ball_tr, &res) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return BALL_HIT_TR;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                          &ball_bl, &res) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return BALL_HIT_BL;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                          &ball_br, &res) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return BALL_HIT_BR;
                    }
                }
            }
        }
    }
    return BALL_HIT_NONE;
}
