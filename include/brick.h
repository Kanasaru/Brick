#ifndef BRICK_H_
#define BRICK_H_

#define BRICK_WIDTH     40
#define BRICK_HEIGHT    20

#define BRICK_BLUE      1
#define BRICK_GREEN     2
#define BRICK_YELLOW    3
#define BRICK_RED       4
#define BRICK_PURPLE    5


Brick create_brick(int x, int y, int hd, float bn, int r, int g, int b);
void draw_brick(Brick br);
void load_bricks(const int level[MAX_ROWS][MAX_COLS]);

#endif /* !BRICK_H_ */
