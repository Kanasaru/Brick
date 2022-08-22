#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

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
            Mix_PlayChannel(-1, media.sounds.wall_hit, 0);
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
            Mix_PlayChannel(-1, media.sounds.wall_hit, 0);
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
            Mix_PlayChannel(-1, media.sounds.wall_hit, 0);
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
            Mix_PlayChannel(-1, media.sounds.wall_hit, 0);
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
            Mix_PlayChannel(-1, media.sounds.wall_hit, 0);
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
            Mix_PlayChannel(-1, media.sounds.wall_hit, 0);
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
                            if (ball.state.MODE == 1)
                                app.bricks[i][j].hardness = 0;
                            else
                                app.bricks[i][j].hardness -= 1;
                            if (app.bricks[i][j].hardness == 0) {
                                app.points += app.bricks[i][j].points;
                                create_drop(app.bricks[i][j].rect.x, 
                                            app.bricks[i][j].rect.y, 
                                            app.bricks[i][j].bonus);
                                Mix_PlayChannel(-1, media.sounds.br_destroy, 0);
                            }
                            else {
                                Mix_PlayChannel(-1, media.sounds.br_hit, 0);
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
                            if (ball.state.MODE == 1)
                                app.bricks[i][j].hardness = 0;
                            else
                                app.bricks[i][j].hardness -= 1;
                            if (app.bricks[i][j].hardness == 0) {
                                app.points += app.bricks[i][j].points;
                                create_drop(app.bricks[i][j].rect.x, 
                                            app.bricks[i][j].rect.y, 
                                            app.bricks[i][j].bonus);
                                Mix_PlayChannel(-1, media.sounds.br_destroy, 0);
                            }
                            else {
                                Mix_PlayChannel(-1, media.sounds.br_hit, 0);
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
                            if (ball.state.MODE == 1)
                                app.bricks[i][j].hardness = 0;
                            else
                                app.bricks[i][j].hardness -= 1;
                            if (app.bricks[i][j].hardness == 0) {
                                app.points += app.bricks[i][j].points;
                                create_drop(app.bricks[i][j].rect.x, 
                                            app.bricks[i][j].rect.y, 
                                            app.bricks[i][j].bonus);
                                Mix_PlayChannel(-1, media.sounds.br_destroy, 0);
                            }
                            else {
                                Mix_PlayChannel(-1, media.sounds.br_hit, 0);
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
                            if (ball.state.MODE == 1)
                                app.bricks[i][j].hardness = 0;
                            else
                                app.bricks[i][j].hardness -= 1;
                            if (app.bricks[i][j].hardness == 0) {
                                app.points += app.bricks[i][j].points;
                                create_drop(app.bricks[i][j].rect.x, 
                                            app.bricks[i][j].rect.y, 
                                            app.bricks[i][j].bonus);
                                Mix_PlayChannel(-1, media.sounds.br_destroy, 0);
                            }
                            else {
                                Mix_PlayChannel(-1, media.sounds.br_hit, 0);
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
                                if (ball.state.MODE == 1)
                                    app.bricks[i][j].hardness = 0;
                                else
                                    app.bricks[i][j].hardness -= 1;
                                if (app.bricks[i][j].hardness == 0) {
                                    app.points += app.bricks[i][j].points;
                                    create_drop(app.bricks[i][j].rect.x, 
                                                app.bricks[i][j].rect.y, 
                                                app.bricks[i][j].bonus);
                                    Mix_PlayChannel(-1, media.sounds.br_destroy, 0);
                                }
                                else {
                                    Mix_PlayChannel(-1, media.sounds.br_hit, 0);
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
                                if (ball.state.MODE == 1)
                                    app.bricks[i][j].hardness = 0;
                                else
                                    app.bricks[i][j].hardness -= 1;
                                if (app.bricks[i][j].hardness == 0) {
                                    app.points += app.bricks[i][j].points;
                                    create_drop(app.bricks[i][j].rect.x, 
                                                app.bricks[i][j].rect.y, 
                                                app.bricks[i][j].bonus);
                                    Mix_PlayChannel(-1, media.sounds.br_destroy, 0);
                                }
                                else {
                                    Mix_PlayChannel(-1, media.sounds.br_hit, 0);
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
                                if (ball.state.MODE == 1)
                                    app.bricks[i][j].hardness = 0;
                                else
                                    app.bricks[i][j].hardness -= 1;
                                if (app.bricks[i][j].hardness == 0) {
                                    app.points += app.bricks[i][j].points;
                                    create_drop(app.bricks[i][j].rect.x, 
                                                app.bricks[i][j].rect.y, 
                                                app.bricks[i][j].bonus);
                                    Mix_PlayChannel(-1, media.sounds.br_destroy, 0);
                                }
                                else {
                                    Mix_PlayChannel(-1, media.sounds.br_hit, 0);
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
                                if (ball.state.MODE == 1)
                                    app.bricks[i][j].hardness = 0;
                                else
                                    app.bricks[i][j].hardness -= 1;
                                if (app.bricks[i][j].hardness == 0) {
                                    app.points += app.bricks[i][j].points;
                                    create_drop(app.bricks[i][j].rect.x, 
                                                app.bricks[i][j].rect.y, 
                                                app.bricks[i][j].bonus);
                                    Mix_PlayChannel(-1, media.sounds.br_destroy, 0);
                                }
                                else {
                                    Mix_PlayChannel(-1, media.sounds.br_hit, 0);
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

        move_drops();
    }
}

void ball_lost(void)
{
    Mix_PlayChannel(-1, media.sounds.lost, 0);
    if (app.lives == 1) {
        /* game over */
        app.lives = STARTING_LIVES;
    }
    else {
        app.lives -= 1;
    }
    app.state.RUNNING = 0;
    reset_drops();
    reset_board();
    reset_ball();
}

void reset_ball(void)
{
    ball.radius = BALL_RADIUS;
    ball.speed = BALL_SPEED;
    ball.color = (SDL_Color) { 255, 255, 255, 255 };
    ball.state.MOVE = BALL_MOVE_NO;
    ball.state.MODE = BALL_MODE_NORMAL;
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

void create_drop(int x, int y, int bp)
{
    int bp_val = (rand() % 100);

    if (bp >= bp_val) {
        for (int i = 0; i < MAX_DROPS; ++i) {
            if (app.drops[i].type == DROP_EMPTY) {
                app.drops[i].radius = DROP_RADIUS;
                app.drops[i].speed = DROP_SPEED;
                app.drops[i].type = (rand() % 8) + 1;
                if (app.drops[i].type == DROP_BOARD_L ||
                    app.drops[i].type == DROP_BOARD_S) {
                    app.drops[i].color = (SDL_Color) { 0, 0, 255, 255 };
                }
                if (app.drops[i].type == DROP_BALL_F ||
                    app.drops[i].type == DROP_BALL_S) {
                    app.drops[i].color = (SDL_Color) { 127, 0, 255, 255 };
                }
                if (app.drops[i].type == DROP_BOARD_F ||
                    app.drops[i].type == DROP_BOARD_SL) {
                    app.drops[i].color = (SDL_Color) { 0, 128, 255, 255 };
                }
                if (app.drops[i].type == DROP_BALL_GM ||
                    app.drops[i].type == DROP_EXTRA_LIVE) {
                    app.drops[i].color = (SDL_Color) { 255, 0, 127, 255 };
                }
                app.drops[i].pos.x = x;
                app.drops[i].pos.y = y;
                app.drops[i].hitbox = (SDL_Rect) { 
                    app.drops[i].pos.x - app.drops[i].radius, 
                    app.drops[i].pos.y - app.drops[i].radius, 
                    app.drops[i].radius * 2 + 1, 
                    app.drops[i].radius * 2 + 1 };
                
                i = MAX_DROPS;
            }
        }
    }
}

void move_drops(void)
{
    for (int i = 0; i < MAX_DROPS; ++i) {
        if (app.drops[i].type != DROP_EMPTY) { 
            int pos_x = app.drops[i].pos.x;
            int pos_y = app.drops[i].pos.y;
            int hb_w = app.drops[i].hitbox.w;
            int hb_h = app.drops[i].hitbox.h;

            SDL_Rect res;

            SDL_Rect cp_b = { pos_x + hb_w / 2, pos_y + hb_h, 1, 1 };
            SDL_Rect cp_l = { pos_x, pos_y + hb_h / 2, 1, 1 };
            SDL_Rect cp_r = { pos_x + hb_w, pos_y + hb_h / 2, 1, 1 };

            if (SDL_IntersectRect(&board.rect, &cp_b, &res) == SDL_TRUE || 
                SDL_IntersectRect(&board.rect, &cp_l, &res) == SDL_TRUE || 
                SDL_IntersectRect(&board.rect, &cp_r, &res) == SDL_TRUE) {
                
                Mix_PlayChannel(-1, media.sounds.effect, 0);

                /* effects */
                switch (app.drops[i].type)
                {
                case DROP_BOARD_L:
                    board.rect.w += (int) (board.rect.w * EFF_BO_LONGER);
                    break;
                case DROP_BOARD_S:
                    board.rect.w -= (int) (board.rect.w * EFF_BO_SMALLER);
                    break;
                case DROP_BALL_F:
                    ball.speed += EFF_BA_SPEED_UP;
                    break;
                case DROP_BALL_S:
                    ball.speed -= EFF_BA_SPEED_DOWN;
                    break;
                case DROP_BOARD_F:
                    ball.speed += EFF_BO_SPEED_UP;
                    break;
                case DROP_BOARD_SL:
                    ball.speed -= EFF_BO_SPEED_DOWN;
                    break;
                case DROP_BALL_GM:
                    ball.state.MODE = BALL_MODE_GM;
                    break;
                case DROP_EXTRA_LIVE:
                    app.lives += 1;
                    break;
                }

                app.drops[i] = create_empty_drop();
            }
            else if (cp_b.y >= SCREEN_HEIGHT) {
                app.drops[i] = create_empty_drop();
            }
            else {
                app.drops[i].pos.y += DROP_SPEED * app.dev.dt;
                app.drops[i].hitbox.y += DROP_SPEED * app.dev.dt;
            }
        }
    }
}

void reset_drops()
{
    for (int i = 0; i < MAX_DROPS; ++i) {
        app.drops[i] = create_empty_drop();
    }
}

Drop create_empty_drop(void)
{
    Drop empty_drop;

    empty_drop.type = DROP_EMPTY;
    empty_drop.radius = 0;
    empty_drop.hitbox = (SDL_Rect) { 0, 0, 0, 0 };
    empty_drop.pos.x = 0;
    empty_drop.pos.y = 0;
    empty_drop.speed = 0;
    empty_drop.color = (SDL_Color) { 0, 0, 0, 0 };

    return empty_drop;
}
