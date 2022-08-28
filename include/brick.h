#ifndef BRICK_H_
#define BRICK_H_

enum brick_types {
    BRICK_NONE,
    BRICK_BLUE,
    BRICK_GREEN,
    BRICK_YELLOW,
    BRICK_RED,
    BRICK_PURPLE
};

/* creates a brick
 * x, y = position (top-left-corner)
 * hd = hardness
 * bn = bonus value
 * r, g, b = color
 */
Brick create_brick(int x, int y, int hd, float bn, int r, int g, int b);
/* loads bricks from given level array */
void load_level(const int level[MAX_ROWS][MAX_COLS]);
void load_bricks(int level);

#endif /* !BRICK_H_ */
