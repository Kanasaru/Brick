#ifndef LOGIC_H_
#define LOGIC_H_

extern App app;

void calc_fpsdt(void);
void run_logic(void);
void run_board_logic(void);
void run_ball_logic(void);
void reset_ball(void);
int board_collision(void);
void move_ball(int direction);
void ball_lost(void);
void reset_board(void);

#endif /* !LOGIC_H_ */
