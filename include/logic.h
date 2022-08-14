#ifndef LOGIC_H_
#define LOGIC_H_

extern App app;

/* calculates fps, delta time and time for fps capping */
void fps_dt(void);
/* runs main logic and all other logic handlers */
void run_logic(void);
/* runs board logic */
void run_board_logic(void);
/* runs ball logic */
void run_ball_logic(void);
/* resets the balls position to it's starting point */
void reset_ball(void);
/* checks if ball and board collided  */
int board_collision(void);
void move_ball(int direction);
/* runs routine if a ball is lost */
void ball_lost(void);
/* resets the boards position to it's starting point */
void reset_board(void);
/* checks if ball and brick collided */
int brick_collision(void);

#endif /* !LOGIC_H_ */
