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
        
        /* 0 = none, 1 = left, 2 = right, 3 = top, 4 = bottom, 5 = tl, 6 = tr, 7 = bl, 8 = br */
        int brick_hit = brick_collision();
        int board_hit = board_collision();
        
        switch (ball.state.MOVE) {
            case BALL_MOVE_TL:
                if (ball.pos.x <= 0) {
                    ball.state.MOVE = BALL_MOVE_TR;
                }
                else if (ball.pos.y - PANEL_HEIGHT <= 0) {
                    ball.state.MOVE = BALL_MOVE_BL;
                }
                else if (brick_hit != 0) {
                    switch(brick_hit) {
                        case 1:
                            ball.state.MOVE = BALL_MOVE_TR;
                            break;
                        case 2:
                            break;
                        case 3:
                            ball.state.MOVE = BALL_MOVE_BL;
                            break;
                        case 4:
                            break;
                        case 5:
                            ball.state.MOVE = BALL_MOVE_BR;
                            break;
                        case 6:
                            break;
                        case 7:
                            break;
                        case 8:
                            break;
                    }
                    move_ball(ball.state.MOVE);
                }
                else {
                    move_ball(BALL_MOVE_TL);
                }
                break;
            case BALL_MOVE_TR:
                if (ball.pos.x >= SCREEN_WIDTH) {
                    ball.state.MOVE = BALL_MOVE_TL;
                }
                else if (ball.pos.y - PANEL_HEIGHT <= 0) {
                    ball.state.MOVE = BALL_MOVE_BR;
                }
                else if (brick_hit != 0) {
                    switch(brick_hit) {
                        case 1:
                            break;
                        case 2:
                            ball.state.MOVE = BALL_MOVE_TL;
                            break;
                        case 3:
                            ball.state.MOVE = BALL_MOVE_BR;
                            break;
                        case 4:
                            break;
                        case 5:
                            break;
                        case 6:
                            ball.state.MOVE = BALL_MOVE_BL;
                            break;
                        case 7:
                            break;
                        case 8:
                            break;
                    }
                    move_ball(ball.state.MOVE);
                }
                else {
                    move_ball(BALL_MOVE_TR);
                }
                break;
            case BALL_MOVE_BL:
                if (ball.pos.x <= 0) {
                    ball.state.MOVE = BALL_MOVE_BR;
                }
                else if (ball.pos.y >= SCREEN_HEIGHT) {
                    ball_lost();
                }
                else if (board_hit != 0) {
                    switch(board_hit) {
                        case 1:
                            ball.state.MOVE = BALL_MOVE_TL;
                            break;
                        case 2:
                            ball.state.MOVE = BALL_MOVE_TR;
                            break;
                        case 3:
                            break;
                    }
                    move_ball(ball.state.MOVE);
                }
                else if (brick_hit != 0) {
                    switch(brick_hit) {
                        case 1:
                            ball.state.MOVE = BALL_MOVE_BR;
                            break;
                        case 2:
                            break;
                        case 3:
                            break;
                        case 4:
                            ball.state.MOVE = BALL_MOVE_TL;
                            break;
                        case 5:
                            break;
                        case 6:
                            break;
                        case 7:
                            ball.state.MOVE = BALL_MOVE_TR;
                            break;
                        case 8:
                            break;
                    }
                    move_ball(ball.state.MOVE);
                }
                else {
                    move_ball(BALL_MOVE_BL);
                }
                break;
            case BALL_MOVE_BR:
                if (ball.pos.x >= SCREEN_WIDTH) {
                    ball.state.MOVE = BALL_MOVE_BL;
                }
                else if (ball.pos.y >= SCREEN_HEIGHT) {
                    ball_lost();
                }
                else if (board_hit != 0) {
                    switch(board_hit) {
                        case 1:
                            ball.state.MOVE = BALL_MOVE_TR;
                            break;
                        case 2:
                            break;
                        case 3:
                            ball.state.MOVE = BALL_MOVE_TL;
                            break;
                    }
                    move_ball(ball.state.MOVE);
                }
                else if (brick_hit != 0) {
                    /* 0 = none, 1 = left, 2 = right, 3 = top, 4 = bottom, 5 = tl, 6 = tr, 7 = bl, 8 = br */
                    switch(brick_hit) {
                        case 1:
                            break;
                        case 2:
                            ball.state.MOVE = BALL_MOVE_BL;
                            break;
                        case 3:
                            break;
                        case 4:
                            ball.state.MOVE = BALL_MOVE_TR;
                            break;
                        case 5:
                            break;
                        case 6:
                            break;
                        case 7:
                            break;
                        case 8:
                            ball.state.MOVE = BALL_MOVE_TL;
                            break;
                    }
                    move_ball(ball.state.MOVE);
                }
                else {
                    move_ball(BALL_MOVE_BR);
                }
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
    
    SDL_Rect ball_p_bottom  = { ball.hitbox.x + ball.hitbox.w / 2, ball.hitbox.y + ball.hitbox.h, 1, 1 };
    SDL_Rect ball_p_c_bl    = { ball.hitbox.x, ball.hitbox.y + ball.hitbox.h, 1, 1 };
    SDL_Rect ball_p_c_br    = { ball.hitbox.x + ball.hitbox.w, ball.hitbox.y + ball.hitbox.h, 1, 1 };
    
    SDL_Rect res_rect;
    
    if (SDL_IntersectRect(&board.rect, &ball.hitbox, &res_rect) == SDL_TRUE) {
        if (SDL_IntersectRect(&board.rect, &ball_p_bottom, &res_rect) == SDL_TRUE) {
            return 1;
        }
        if (SDL_IntersectRect(&board.rect, &ball_p_c_bl, &res_rect) == SDL_TRUE) {
            return 2;
        }
        if (SDL_IntersectRect(&board.rect, &ball_p_c_br, &res_rect) == SDL_TRUE) {
            return 3;
        }
    }
    
    return 0;
}

int brick_collision(void)
{
    /* 0 = none, 1 = left, 2 = right, 3 = top, 4 = bottom, 5 = tl, 6 = tr, 7 = bl, 8 = br */
    SDL_Rect ball_p_top     = { ball.hitbox.x + ball.hitbox.w / 2, ball.hitbox.y, 1, 1 };
    SDL_Rect ball_p_bottom  = { ball.hitbox.x + ball.hitbox.w / 2, ball.hitbox.y + ball.hitbox.h, 1, 1 };
    SDL_Rect ball_p_left    = { ball.hitbox.x, ball.hitbox.y + ball.hitbox.h / 2, 1, 1 };
    SDL_Rect ball_p_right   = { ball.hitbox.x + ball.hitbox.w, ball.hitbox.y + ball.hitbox.h / 2, 1, 1 };
    
    SDL_Rect ball_p_c_tl    = { ball.hitbox.x + BALL_CORNER_AC, ball.hitbox.y + BALL_CORNER_AC, 1, 1 };
    SDL_Rect ball_p_c_tr    = { ball.hitbox.x + ball.hitbox.w - BALL_CORNER_AC, ball.hitbox.y + BALL_CORNER_AC, 1, 1 };
    SDL_Rect ball_p_c_bl    = { ball.hitbox.x + BALL_CORNER_AC, ball.hitbox.y + ball.hitbox.h - BALL_CORNER_AC, 1, 1 };
    SDL_Rect ball_p_c_br    = { ball.hitbox.x + ball.hitbox.w - BALL_CORNER_AC, ball.hitbox.y + ball.hitbox.h - BALL_CORNER_AC, 1, 1 };
    
    SDL_Rect res_rect;

    for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_COLS; ++j) {
            /* ignore empty ones */
            if (app.bricks[i][j].hardness > 0) {
                if (SDL_IntersectRect(&app.bricks[i][j].rect, &ball.hitbox, &res_rect) == SDL_TRUE) {
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, &ball_p_left, &res_rect) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return 1;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, &ball_p_right, &res_rect) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return 2;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, &ball_p_top, &res_rect) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return 3;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, &ball_p_bottom, &res_rect) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return 4;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, &ball_p_c_tl, &res_rect) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return 5;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, &ball_p_c_tr, &res_rect) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return 6;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, &ball_p_c_bl, &res_rect) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return 7;
                    }
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, &ball_p_c_br, &res_rect) == SDL_TRUE) {
                        app.bricks[i][j].hardness -= 1;
                        return 8;
                    }
                }
            }
        }
    }
    
    return 0;
}
