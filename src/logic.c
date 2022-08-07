#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
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
    
    factor = (float) FPS / fps; /*fps / FPS;*/
    /* strip decimal places to 2
     * factor = (int) (factor * 100);
     * factor = factor / 100;
     */
    
    return factor;
}

void run_logic(void)
{
    calc_fps();
}
