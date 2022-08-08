#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
#include "event.h"

void handle_events(void)
{
    SDL_Event event;
        
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                app.events.QUIT_GAME = 1;
                break;
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ) {
                    case SDLK_LEFT:
                        app.events.KEY_LEFT = 1;
                        app.events.KEY_RIGHT = 0;
                        break;
                    case SDLK_RIGHT:
                        app.events.KEY_RIGHT = 1;
                        app.events.KEY_LEFT = 0;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch( event.key.keysym.sym ) {
                    case SDLK_ESCAPE:
                        app.events.QUIT_GAME = 1;
                        break;
                    case SDLK_LEFT:
                        app.events.KEY_LEFT = 0;
                        break;
                    case SDLK_RIGHT:
                        app.events.KEY_RIGHT = 0;
                        break;
                    case SDLK_SPACE:
                        app.events.KEY_SPACE = 1;
                        break;
                    case SDLK_p:
                        app.events.PAUSE_GAME = 1;
                        break;
                    case SDLK_PAUSE:
                        app.events.PAUSE_GAME = 1;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
}

void clear_events(void)
{
    app.events.PAUSE_GAME = 0;
    app.events.QUIT_GAME = 0;
    app.events.KEY_SPACE = 0;
}
