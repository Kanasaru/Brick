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
        handle_events();
        
        run_logic();
        
        draw();
    }
    
    clr_init();
    
    return 0;
}
