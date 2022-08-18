#ifndef LOGIC_H_
#define LOGIC_H_

extern App app;

enum ball_hit {
    BALL_HIT_NONE,
    BALL_HIT_LEFT,
    BALL_HIT_RIGHT,
    BALL_HIT_TOP,
    BALL_HIT_BOTTOM,
    BALL_HIT_TL,
    BALL_HIT_TR,
    BALL_HIT_BL,
    BALL_HIT_BR
};

enum ball_move {
    BALL_MOVE_NO,
    BALL_MOVE_TL,
    BALL_MOVE_TR,
    BALL_MOVE_BL,
    BALL_MOVE_BR,
    BALL_MOVE_WAIT
};

enum board_move {
    BOARD_MOVE_NO,
    BOARD_MOVE_TL,
    BOARD_MOVE_TR
};

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
/* moves the ball about x and y in given given direction  */
void move_ball(int x, int y, int direction);
/* runs routine if a ball is lost */
void ball_lost(void);
/* resets the boards position to it's starting point */
void reset_board(void);

#endif /* !LOGIC_H_ */
