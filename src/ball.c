#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
#include "ball.h"

void draw_ball(SDL_Renderer *renderer, int hitbox)
{
    if (hitbox == 1) {
        draw_ball_hitbox(renderer);
    }
    
    SDL_SetRenderDrawColor(renderer, 
                           ball.color.r, 
                           ball.color.g, 
                           ball.color.b, 
                           ball.color.a);
    
    for (int w = 0; w < ball.radius * 2; w++)
    {
        for (int h = 0; h < ball.radius * 2; h++)
        {
            int dx = ball.radius - w;
            int dy = ball.radius - h;
            if ((dx*dx + dy*dy) < (ball.radius * ball.radius))
            {
                SDL_RenderDrawPoint(renderer, 
                                    ball.pos.x + dx, ball.pos.y + dy);
            }
        }
    }
}

void draw_ball_hitbox(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 199, 36, 177, 255);
    SDL_RenderDrawRect(renderer, &ball.hitbox);
}
