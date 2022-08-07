#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
#include "board.h"

void draw_board(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 
                           board.color.r, 
                           board.color.g, 
                           board.color.b, 
                           board.color.a);
    SDL_RenderFillRect(renderer, &board.rect);
    
    SDL_SetRenderDrawColor(app.renderer, 64, 64, 64, 255);
    SDL_RenderDrawLine(app.renderer, 
                       board.rect.x, 
                       board.rect.y, 
                       board.rect.x + BOARD_WIDTH - 1, 
                       board.rect.y);
    SDL_RenderDrawLine(app.renderer, 
                       board.rect.x, 
                       board.rect.y, 
                       board.rect.x, 
                       board.rect.y + BOARD_HEIGHT - 1);
    
    SDL_SetRenderDrawColor(app.renderer, 100, 100, 100, 255);
    SDL_RenderDrawLine(app.renderer, 
                       board.rect.x, 
                       board.rect.y + BOARD_HEIGHT - 1, 
                       board.rect.x + BOARD_WIDTH - 1, 
                       board.rect.y + BOARD_HEIGHT - 1);
    SDL_RenderDrawLine(app.renderer, 
                       board.rect.x + BOARD_WIDTH - 1, 
                       board.rect.y + BOARD_HEIGHT - 1, 
                       board.rect.x + BOARD_WIDTH - 1, 
                       board.rect.y);
}
