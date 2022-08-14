#ifndef RENDER_H_
#define RENDER_H_

extern App app;
extern Board board;
extern Ball ball;

/* handles all drawing and calls to drawing functions */
void draw(void);
/* prepares the screen for drawing */
void prepare_screen(void);
/* switch surfaces */
void present_screen(void);
/* draws all bricks with hardness higher than 0 */
void draw_current_bricks(void);
/* draws a single brick by given brick */
void draw_brick(Brick br);
/* draws the board */
void draw_board(void);
/* draws the panel */
void draw_panel(void);
/* draws the ball */
void draw_ball(int hitbox);
/* draws the balls hitbox */
void draw_ball_hitbox(void);

#endif /* !RENDER_H_ */
