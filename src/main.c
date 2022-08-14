#include <SDL2/SDL.h>

#include "defs.h"
#include "structs.h"
#include "init.h"
#include "event.h"
#include "logic.h"
#include "render.h"

int main(int argc, char **argv)
{
    init_SDL();
    init_game();
    
    while (app.state.QUIT == 0) {
        
        fps_dt();
        
        handle_events();
    
        run_logic();
        
        draw();
        
        SDL_Delay(app.dev.frc);
    }
    
    clr_init();
    
    return EXIT_SUCCESS;
}
