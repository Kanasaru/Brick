#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "defs.h"
#include "structs.h"
#include "render.h"

void prepareScene(void)
{
    SDL_SetRenderDrawColor(app.renderer, 36, 36, 36, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void)
{
    SDL_RenderPresent(app.renderer);
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

void draw_brick(Brick br)
{
    SDL_SetRenderDrawColor(app.renderer, 
                           br.color.r, 
                           br.color.g, 
                           br.color.b, 
                           br.color.a);
    SDL_RenderFillRect(app.renderer, &br.rect);
    
    SDL_SetRenderDrawColor(app.renderer, 220, 220, 220, 255);
    SDL_RenderDrawLine(app.renderer, 
                       br.rect.x, 
                       br.rect.y, 
                       br.rect.x + BRICK_WIDTH - 1, 
                       br.rect.y);
    SDL_RenderDrawLine(app.renderer, 
                       br.rect.x, 
                       br.rect.y, 
                       br.rect.x, 
                       br.rect.y + BRICK_HEIGHT - 1);
    
    SDL_SetRenderDrawColor(app.renderer, 128, 128, 128, 255);
    SDL_RenderDrawLine(app.renderer, 
                       br.rect.x , 
                       br.rect.y + BRICK_HEIGHT - 1, 
                       br.rect.x + BRICK_WIDTH - 1, 
                       br.rect.y + BRICK_HEIGHT - 1);
    SDL_RenderDrawLine(app.renderer, 
                       br.rect.x + BRICK_WIDTH - 1, 
                       br.rect.y + BRICK_HEIGHT - 1, 
                       br.rect.x + BRICK_WIDTH - 1, 
                       br.rect.y);
}

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
