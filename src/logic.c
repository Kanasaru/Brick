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
}

void run_board_logic(void)
{
    if (board.state.MOVE != 0) {
        int move_x = (int) (board.speed * app.dev.lgcrt);
        
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
