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
    
    factor = (float) FPS / (float) fps;
    /* strip decimal places to 2
     * factor = (int) (factor * 100);
     * factor = factor / 100;
     */
    
    return factor;
}

void run_logic(void)
{
    calc_fps();
    
    if (app.events.QUIT_GAME == 1) {
        app.state.QUIT = 1;
    }
    
    int move_x = (int) (board.speed * app.dev.lgcrt);
    if (app.events.KEY_LEFT == 1) {
        if (board.rect.x - move_x >= 0) {
            board.rect.x -= move_x;
            if (app.state.RUNNING == 0) {
                ball.pos.x -= move_x;
            }
        }
    } else if (app.events.KEY_RIGHT == 1) {
        if (board.rect.x + move_x + board.rect.w <= SCREEN_WIDTH) {
            board.rect.x += move_x;
            if (app.state.RUNNING == 0) {
                ball.pos.x += move_x;
            }
        }
    }
    
    clear_events();
}
