#include "defs.h"
#include "structs.h"
#include "brick.h"

Brick create_brick(int x, int y, int hd, float bn, int r, int g, int b)
{
    Brick br;
    
    br.rect.x = x;
    br.rect.y = y;
    br.rect.w = BRICK_WIDTH;
    br.rect.h = BRICK_HEIGHT;
    br.hardness = hd;
    br.bonus = bn;
    br.color.r = r;
    br.color.g = g;
    br.color.b = b;
    br.color.a = 255;
    
    return br;
}

void load_bricks(const int level[MAX_ROWS][MAX_COLS])
{
    for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_COLS; ++j) {
            switch (level[i][j])
            {
            case BRICK_BLUE:
                app.bricks[i][j].rect.x = j * BRICK_WIDTH;
                app.bricks[i][j].rect.y = i * BRICK_HEIGHT + PANEL_HEIGHT;
                app.bricks[i][j].rect.w = BRICK_WIDTH;
                app.bricks[i][j].rect.h = BRICK_HEIGHT;
                app.bricks[i][j].hardness = 1;
                app.bricks[i][j].bonus = 0.1;
                app.bricks[i][j].color.r = 0;
                app.bricks[i][j].color.g = 0;
                app.bricks[i][j].color.b = 139;
                app.bricks[i][j].color.a = 255;
                break;
                
            case BRICK_GREEN:
                app.bricks[i][j].rect.x = j * BRICK_WIDTH;
                app.bricks[i][j].rect.y = i * BRICK_HEIGHT + PANEL_HEIGHT;
                app.bricks[i][j].rect.w = BRICK_WIDTH;
                app.bricks[i][j].rect.h = BRICK_HEIGHT;
                app.bricks[i][j].hardness = 2;
                app.bricks[i][j].bonus = 0.2;
                app.bricks[i][j].color.r = 0;
                app.bricks[i][j].color.g = 100;
                app.bricks[i][j].color.b = 0;
                app.bricks[i][j].color.a = 255;
                break;
                
            case BRICK_YELLOW:
                app.bricks[i][j].rect.x = j * BRICK_WIDTH;
                app.bricks[i][j].rect.y = i * BRICK_HEIGHT + PANEL_HEIGHT;
                app.bricks[i][j].rect.w = BRICK_WIDTH;
                app.bricks[i][j].rect.h = BRICK_HEIGHT;
                app.bricks[i][j].hardness = 5;
                app.bricks[i][j].bonus = 0.3;
                app.bricks[i][j].color.r = 255;
                app.bricks[i][j].color.g = 255;
                app.bricks[i][j].color.b = 0;
                app.bricks[i][j].color.a = 255;
                break;
                
            case BRICK_RED:
                app.bricks[i][j].rect.x = j * BRICK_WIDTH;
                app.bricks[i][j].rect.y = i * BRICK_HEIGHT + PANEL_HEIGHT;
                app.bricks[i][j].rect.w = BRICK_WIDTH;
                app.bricks[i][j].rect.h = BRICK_HEIGHT;
                app.bricks[i][j].hardness = 10;
                app.bricks[i][j].bonus = 0.4;
                app.bricks[i][j].color.r = 255;
                app.bricks[i][j].color.g = 0;
                app.bricks[i][j].color.b = 0;
                app.bricks[i][j].color.a = 255;
                break;
                
            case BRICK_PURPLE:
                app.bricks[i][j].rect.x = j * BRICK_WIDTH;
                app.bricks[i][j].rect.y = i * BRICK_HEIGHT + PANEL_HEIGHT;
                app.bricks[i][j].rect.w = BRICK_WIDTH;
                app.bricks[i][j].rect.h = BRICK_HEIGHT;
                app.bricks[i][j].hardness = 10;
                app.bricks[i][j].bonus = 0.5;
                app.bricks[i][j].color.r = 139;
                app.bricks[i][j].color.g = 0;
                app.bricks[i][j].color.b = 139;
                app.bricks[i][j].color.a = 255;
                break;
            
            default:
                app.bricks[i][j].hardness = 0;
                break;
            }
        }
    }
}
