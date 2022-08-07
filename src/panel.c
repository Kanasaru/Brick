#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "defs.h"
#include "structs.h"
#include "panel.h"

void draw_panel(SDL_Renderer *renderer, int fps, float lgcrt, int lvl, int pts)
{
    TTF_Font *font;
    SDL_Surface *text;
    SDL_Texture *text_texture;
    
    int lgth_fps = snprintf(NULL, 0, "FPS: %d (%f) | ", fps, lgcrt);
    int lgth_lvl = snprintf(NULL, 0, "LEVEL: %d | ", lvl);
    int lgth_pts = snprintf(NULL, 0, "POINTS: %d", pts);
    int lgth_str = lgth_fps + lgth_lvl + lgth_pts + 1;
    int padding  = (int) (PANEL_HEIGHT - PANEL_FONT_SIZE * 0.75) / 2 - 1;
    
    SDL_Color color_txt = { 255, 255, 255 };
    SDL_Color color_bg = { 128, 0, 0 };
    SDL_Rect rect = { 0, 0, SCREEN_WIDTH, PANEL_HEIGHT };
    
    char *str = malloc(lgth_str);
    snprintf(str, lgth_str, 
             "FPS: %d (%f) | LEVEL: %d | POINTS: %d", 
             fps, lgcrt, lvl, pts);
    
    SDL_SetRenderDrawColor(renderer, color_bg.r, color_bg.g, color_bg.b, 255);
    SDL_RenderFillRect(renderer, &rect);
    
    font = TTF_OpenFont(STD_FONT, PANEL_FONT_SIZE * 0.75);
    if (!font) {
        printf("%s\n", TTF_GetError());
    }
    
    text = TTF_RenderText_Solid(font, str, color_txt);
    if (!text) {
        printf("%s\n", TTF_GetError());
    }

    text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_Rect dest = { padding, padding, text->w, text->h };
    
    SDL_RenderCopy(renderer, text_texture, 0, &dest);
    
    TTF_CloseFont(font);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text);
    free(str);
}
