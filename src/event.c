#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
#include "event.h"

void handle_events(void)
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
        case SDL_QUIT:
            app.state.QUIT = 1;
            break;
        case SDL_KEYUP:
            switch( event.key.keysym.sym ) {
                case SDLK_ESCAPE:
                    app.state.QUIT = 1;
                    break;
                case SDLK_p:
                    app.state.PAUSE = 1;
                    break;
                case SDLK_PAUSE:
                    app.state.PAUSE = 1;
                    break;
                default:
                    break;
            }
            break;
        }
        handle_board_event(event);
        handle_ball_event(event);
    }
}

void handle_board_event(SDL_Event event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                board.state.MOVE = 1;
                break;
            case SDLK_RIGHT:
                board.state.MOVE = 2;
                break;
            default:
                break;
        }
        break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                if (board.state.MOVE == 1)
                    board.state.MOVE = 0;
                break;
            case SDLK_RIGHT:
            if (board.state.MOVE == 2)
                    board.state.MOVE = 0;
                break;
        }
        break;
    }
}

void handle_ball_event(SDL_Event event)
{
    switch (event.type)
    {
    case SDL_KEYUP:
        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (ball.state.MOVE == 0)
                    ball.state.MOVE = 5;
                    app.state.RUNNING = 1;
                break;
        }
        break;
    }
}
