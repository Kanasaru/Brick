#ifndef LOGIC_H_
#define LOGIC_H_

extern App app;

void calc_fps(void);
float calc_lgcrt(int fps);
void run_logic(void);
void run_board_logic(void);
void run_ball_logic(void);
void reset_ball(void);
int board_collision(int x, int y);

#endif /* !LOGIC_H_ */
