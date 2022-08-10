#ifndef RENDER_H_
#define RENDER_H_

extern App app;
extern Board board;
extern Ball ball;

void draw(void);
void prepare_screen(void);
void present_screen(void);
void draw_current_bricks(void);
void draw_brick(Brick br);
void draw_board(void);
void draw_panel(void);
void draw_ball(int hitbox);
void draw_ball_hitbox(void);

#endif /* !RENDER_H_ */
