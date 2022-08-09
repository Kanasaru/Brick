#ifndef BRICK_H_
#define BRICK_H_

Brick create_brick(int x, int y, int hd, float bn, int r, int g, int b);
void load_bricks(const int level[MAX_ROWS][MAX_COLS]);

#endif /* !BRICK_H_ */
