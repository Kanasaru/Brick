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
    /* is ball waiting for start? */
    if (ball.state.MOVE == BALL_MOVE_WAIT) {
        if (board.state.MOVE == BOARD_MOVE_TL) {
            ball.state.MOVE = BALL_MOVE_TL;
        }
        else if (board.state.MOVE == BOARD_MOVE_TR) {
            ball.state.MOVE = BALL_MOVE_TR;
        }
        else {
            ball.state.MOVE = (rand() % 2) + 1;
        }
    }
    /* is ball moving? */
    if (ball.state.MOVE != BALL_MOVE_NO) {
        /* calculate new hitbox values */
        int move_x = ball.speed * app.dev.dt;
        int move_y = ball.speed * app.dev.dt;
        int pos_x = ball.hitbox.x;
        int pos_y = ball.hitbox.y;
        int hb_w = ball.hitbox.w;
        int hb_h = ball.hitbox.h;
        switch (ball.state.MOVE)
        {
        case BALL_MOVE_TL:
            pos_x -= move_x;
            pos_y -= move_y;
            break;
        case BALL_MOVE_TR:
            pos_x += move_x;
            pos_y -= move_y;
            break;
        case BALL_MOVE_BL:
            pos_x -= move_x;
            pos_y+= move_y;
            break;
        case BALL_MOVE_BR:
            pos_x += move_x;
            pos_y += move_y;
            break;
        }
        /* hitbox */
        SDL_Rect hitbox = { pos_x, pos_y, hb_w, hb_h };
        /* calculate collision points */
        int c_ac = BALL_CORNER_AC;
        SDL_Rect cp_t = { pos_x + hb_w / 2, pos_y, 1, 1 };
        SDL_Rect cp_b = { pos_x + hb_w / 2, pos_y + hb_h, 1, 1 };
        SDL_Rect cp_l = { pos_x, pos_y + hb_h / 2, 1, 1 };
        SDL_Rect cp_r = { pos_x + hb_w, pos_y + hb_h / 2, 1, 1 };
        SDL_Rect cp_tl = { pos_x + c_ac, pos_y + c_ac, 1, 1 };
        SDL_Rect cp_tr = { pos_x + hb_w - c_ac, pos_y + c_ac, 1, 1 };
        SDL_Rect cp_bl = { pos_x + c_ac, pos_y + hb_h - c_ac, 1, 1 };
        SDL_Rect cp_br = { pos_x + hb_w - c_ac, pos_y + hb_h - c_ac, 1, 1 };
        /* collision result rect and hit check */
        SDL_Rect res;
        int hit_heck = 0; /* false */
        int new_direction = BALL_MOVE_NO;
        /* check if ball collides with walls */
        /* left wall */
        if (cp_l.x <= 0) {
            switch (ball.state.MOVE)
            {
            case BALL_MOVE_TL:
                new_direction = BALL_MOVE_TR;
                hit_heck = 1;
                move_x += cp_l.x;
                move_y += cp_l.x;
                break;

            case BALL_MOVE_BL:
                new_direction = BALL_MOVE_BR;
                hit_heck = 1;
                move_x += cp_l.x;
                move_y += cp_l.x;
                break;
            }
        } 
        /* right wall */
        else if (cp_r.x >= SCREEN_WIDTH) {
            switch (ball.state.MOVE)
            {
            case BALL_MOVE_TR:
                new_direction = BALL_MOVE_TL;
                hit_heck = 1;
                move_x -= cp_r.x - SCREEN_WIDTH;
                move_y -= cp_r.x - SCREEN_WIDTH;
                break;

            case BALL_MOVE_BR:
                new_direction = BALL_MOVE_BL;
                hit_heck = 1;
                move_x -= cp_r.x - SCREEN_WIDTH;
                move_y -= cp_r.x - SCREEN_WIDTH;
                break;
            }
        }
        /* panel hit */
        else if (cp_t.y <= PANEL_HEIGHT) {
            switch (ball.state.MOVE)
            {
            case BALL_MOVE_TL:
                new_direction = BALL_MOVE_BL;
                hit_heck = 1;
                move_x -= PANEL_HEIGHT - cp_t.y;
                move_y -= PANEL_HEIGHT - cp_t.y;
                break;

            case BALL_MOVE_TR:
                new_direction = BALL_MOVE_BR;
                hit_heck = 1;
                move_x -= PANEL_HEIGHT - cp_t.y;
                move_y -= PANEL_HEIGHT - cp_t.y;
                break;
            }
        }
        /* bottom */
        else if (cp_b.y >= SCREEN_HEIGHT) {
            ball_lost();
        }
        /* check if ball collides with the board */
        /* top of the board */
        else if (SDL_IntersectRect(&board.rect, &cp_b, &res) == SDL_TRUE) {
            switch (ball.state.MOVE)
            {
            case BALL_MOVE_BL:
                new_direction = BALL_MOVE_TL;
                hit_heck = 1;
                move_x -= res.h;
                move_y -= res.h;
                break;

            case BALL_MOVE_BR:
                new_direction = BALL_MOVE_TR;
                hit_heck = 1;
                move_x -= res.h;
                move_y -= res.h;
                break;
            }
        }
        /* left corner of the board*/
        else if (SDL_IntersectRect(&board.rect, &cp_br, &res) == SDL_TRUE) {
            switch (ball.state.MOVE)
            {
            case BALL_MOVE_BL:
                new_direction = BALL_MOVE_TL;
                hit_heck = 1;
                if (res.h > res.w) {
                    move_x -= res.h;
                    move_y -= res.h;
                }
                else {
                    move_x -= res.w;
                    move_y -= res.w;
                }
                break;

            case BALL_MOVE_BR:
                new_direction = BALL_MOVE_TL;
                hit_heck = 1;
                if (res.h > res.w) {
                    move_x -= res.h;
                    move_y -= res.h;
                }
                else {
                    move_x -= res.w;
                    move_y -= res.w;
                }
                break;
            }
        }
        /* right corner of the board*/
        else if (SDL_IntersectRect(&board.rect, &cp_bl, &res) == SDL_TRUE) {
            switch (ball.state.MOVE)
            {
            case BALL_MOVE_BL:
                new_direction = BALL_MOVE_TR;
                hit_heck = 1;
                if (res.h > res.w) {
                    move_x -= res.h;
                    move_y -= res.h;
                }
                else {
                    move_x -= res.w;
                    move_y -= res.w;
                }
                break;

            case BALL_MOVE_BR:
                new_direction = BALL_MOVE_TR;
                hit_heck = 1;
                if (res.h > res.w) {
                    move_x -= res.h;
                    move_y -= res.h;
                }
                else {
                    move_x -= res.w;
                    move_y -= res.w;
                }
                break;
            }
        }
        /* check if ball collides with a brick */
        for (int i = 0; i < MAX_ROWS; ++i) {
            for (int j = 0; j < MAX_COLS; ++j) {
                if (app.bricks[i][j].hardness > 0) {
                    if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                        &hitbox, &res) == SDL_TRUE) {
                        /* right brick side*/
                        if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                            &cp_l, &res) == SDL_TRUE) {
                            app.bricks[i][j].hardness -= 1;
                            if (app.bricks[i][j].hardness == 0) {
                                app.points += app.bricks[i][j].points;
                            }
                            switch (ball.state.MOVE)
                            {
                            case BALL_MOVE_TL:
                                new_direction = BALL_MOVE_TR;
                                hit_heck = 1;
                                move_x -= res.w;
                                move_y -= res.w;
                                break;

                            case BALL_MOVE_BL:
                                new_direction = BALL_MOVE_BR;
                                hit_heck = 1;
                                move_x -= res.w;
                                move_y -= res.w;
                                break;
                            }
                        }
                        /* left brick side */
                        else if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                            &cp_r, &res) == SDL_TRUE) {
                            app.bricks[i][j].hardness -= 1;
                            if (app.bricks[i][j].hardness == 0) {
                                app.points += app.bricks[i][j].points;
                            }
                            switch (ball.state.MOVE)
                            {
                            case BALL_MOVE_TR:
                                new_direction = BALL_MOVE_TL;
                                hit_heck = 1;
                                move_x -= res.w;
                                move_y -= res.w;
                                break;

                            case BALL_MOVE_BR:
                                new_direction = BALL_MOVE_BL;
                                hit_heck = 1;
                                move_x -= res.w;
                                move_y -= res.w;
                                break;
                            }
                        }
                        /* bottom brick hit */
                        else if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                            &cp_t, &res) == SDL_TRUE) {
                            app.bricks[i][j].hardness -= 1;
                            if (app.bricks[i][j].hardness == 0) {
                                app.points += app.bricks[i][j].points;
                            }
                            switch (ball.state.MOVE)
                            {
                            case BALL_MOVE_TL:
                                new_direction = BALL_MOVE_BL;
                                hit_heck = 1;
                                move_x -= res.h;
                                move_y -= res.h;
                                break;

                            case BALL_MOVE_TR:
                                new_direction = BALL_MOVE_BR;
                                hit_heck = 1;
                                move_x -= res.h;
                                move_y -= res.h;
                                break;
                            }
                        }
                        /* top brick hit */
                        else if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                            &cp_b, &res) == SDL_TRUE) {
                            app.bricks[i][j].hardness -= 1;
                            if (app.bricks[i][j].hardness == 0) {
                                app.points += app.bricks[i][j].points;
                            }
                            switch (ball.state.MOVE)
                            {
                            case BALL_MOVE_BL:
                                new_direction = BALL_MOVE_TL;
                                hit_heck = 1;
                                move_x -= res.h;
                                move_y -= res.h;
                                break;

                            case BALL_MOVE_BR:
                                new_direction = BALL_MOVE_TR;
                                hit_heck = 1;
                                move_x -= res.h;
                                move_y -= res.h;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if(!hit_heck) {
            for (int i = 0; i < MAX_ROWS; ++i) {
                for (int j = 0; j < MAX_COLS; ++j) {
                    if (app.bricks[i][j].hardness > 0) {
                        if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                            &hitbox, &res) == SDL_TRUE) {
                            /* bottom right corner */
                            if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                                &cp_tl, &res) == SDL_TRUE) {
                                app.bricks[i][j].hardness -= 1;
                                if (app.bricks[i][j].hardness == 0) {
                                    app.points += app.bricks[i][j].points;
                                }
                                switch (ball.state.MOVE)
                                {
                                case BALL_MOVE_TL:
                                    new_direction = BALL_MOVE_BR;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;

                                case BALL_MOVE_BL:
                                    new_direction = BALL_MOVE_BR;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;
                                
                                case BALL_MOVE_TR:
                                    new_direction = BALL_MOVE_BR;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;
                                }
                            }
                            /* bottom left corner */
                            else if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                                &cp_tr, &res) == SDL_TRUE) {
                                app.bricks[i][j].hardness -= 1;
                                if (app.bricks[i][j].hardness == 0) {
                                    app.points += app.bricks[i][j].points;
                                }
                                switch (ball.state.MOVE)
                                {
                                case BALL_MOVE_TR:
                                    new_direction = BALL_MOVE_BL;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;

                                case BALL_MOVE_BR:
                                    new_direction = BALL_MOVE_BL;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;

                                case BALL_MOVE_TL:
                                    new_direction = BALL_MOVE_BL;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;
                                }
                            }
                            /* top right corner */
                            else if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                                &cp_bl, &res) == SDL_TRUE) {
                                app.bricks[i][j].hardness -= 1;
                                if (app.bricks[i][j].hardness == 0) {
                                    app.points += app.bricks[i][j].points;
                                }
                                switch (ball.state.MOVE)
                                {
                                case BALL_MOVE_BL:
                                    new_direction = BALL_MOVE_TR;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;

                                case BALL_MOVE_BR:
                                    new_direction = BALL_MOVE_TR;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;

                                case BALL_MOVE_TL:
                                    new_direction = BALL_MOVE_TR;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;
                                }
                            }
                            /* top left corner */
                            else if (SDL_IntersectRect(&app.bricks[i][j].rect, 
                                                &cp_br, &res) == SDL_TRUE) {
                                app.bricks[i][j].hardness -= 1;
                                if (app.bricks[i][j].hardness == 0) {
                                    app.points += app.bricks[i][j].points;
                                }
                                switch (ball.state.MOVE)
                                {
                                case BALL_MOVE_BL:
                                    new_direction = BALL_MOVE_TL;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;

                                case BALL_MOVE_BR:
                                    new_direction = BALL_MOVE_TL;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;

                                case BALL_MOVE_TR:
                                    new_direction = BALL_MOVE_TL;
                                    hit_heck = 1;
                                    if (res.h > res.w) {
                                        move_x -= res.h;
                                        move_y -= res.h;
                                    }
                                    else {
                                        move_x -= res.w;
                                        move_y -= res.w;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        /* collision detected? */
        if (hit_heck) {
            /* move the ball and set new direction */
            move_ball(move_x, move_y, ball.state.MOVE);
            ball.state.MOVE = new_direction;
        }
        /* not collision, then just move the ball */
        else {
            move_ball(move_x, move_y, ball.state.MOVE);
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

void move_ball(int x, int y, int direction)
{  
    switch (direction) {
        case BALL_MOVE_TL:
            ball.pos.x -= x;
            ball.pos.y -= y;
            ball.hitbox.x -= x;
            ball.hitbox.y -= y;
            break;
        case BALL_MOVE_TR:
            ball.pos.x += x;
            ball.pos.y -= y;
            ball.hitbox.x += x;
            ball.hitbox.y -= y;
            break;
        case BALL_MOVE_BL:
            ball.pos.x -= x;
            ball.pos.y += y;
            ball.hitbox.x -= x;
            ball.hitbox.y += y;
            break;
        case BALL_MOVE_BR:
            ball.pos.x += x;
            ball.pos.y += y;
            ball.hitbox.x += x;
            ball.hitbox.y += y;
            break;
    }
}
