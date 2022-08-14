#ifndef BRICK_H_
#define BRICK_H_

/* creates a brick
 * x, y = position (top-left-corner)
 * hd = hardness
 * bn = bonus value
 * r, g, b = color
 */
Brick create_brick(int x, int y, int hd, float bn, int r, int g, int b);
/* loads bricks from given level array */
void load_bricks(const int level[MAX_ROWS][MAX_COLS]);

#endif /* !BRICK_H_ */
