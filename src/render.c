#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
#include "brick.h"
#include "render.h"
#include "panel.h"
#include "board.h"
#include "ball.h"

void prepareScene(void)
{
    SDL_SetRenderDrawColor(app.renderer, 36, 36, 36, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void)
{
    SDL_RenderPresent(app.renderer);
}

void draw_current_bricks(void)
{
    for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_COLS; ++j) {
            if (app.bricks[i][j].hardness > 0) {
                draw_brick(app.bricks[i][j]);
            }
        }
    }
}

void draw(void)
{
    prepareScene();
    
    draw_panel(app.renderer, 
               app.dev.fps, 
               app.dev.lgcrt, 
               app.level, 
               app.points);
    draw_current_bricks();
    draw_board(app.renderer);
    draw_ball(app.renderer, 0);
    
    presentScene();
}
