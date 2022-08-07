#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
#include "init.h"
#include "render.h"
#include "logic.h"

int main(int argc, char **argv)
{
    init_SDL();
    init_game();

    int close = 0;
    while (!close) {
        SDL_Event event;
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = 1;
                    break;
                case SDL_KEYDOWN:
                    switch( event.key.keysym.sym ) {
                        case SDLK_LEFT:
                            board.rect.x -= (int) (board.speed * app.dev.lgcrt);
                            break;
                        case SDLK_RIGHT:
                            board.rect.x += (int) (board.speed * app.dev.lgcrt);
                            break;
                        case SDLK_SPACE:
                            printf( "Start ball!\n" );
                            break;
                        case SDLK_DOWN:
                            board.rect.y += 1;
                            break;
                        default:
                            break;
                        }
                case SDL_KEYUP:
                    break;
            }
        }
        
        run_logic();
        
        draw();
    }
    
    clr_init();
    
    return 0;
}
