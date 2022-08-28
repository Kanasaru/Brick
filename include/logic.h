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

enum ball_modes {
    BALL_MODE_NORMAL,
    BALL_MODE_GM
};

enum board_move {
    BOARD_MOVE_NO,
    BOARD_MOVE_TL,
    BOARD_MOVE_TR
};

enum drops_type {
    DROP_EMPTY,
    DROP_BOARD_L,
    DROP_BOARD_S,
    DROP_BOARD_F,
    DROP_BOARD_SL,
    DROP_BALL_F,
    DROP_BALL_S,
    DROP_BALL_GM,
    DROP_EXTRA_LIVE
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
/* create a drop by given position and bonus possibility */
void create_drop(int x, int y, int bp);
/* moves the drops, ckecks collision and effects */
void move_drops(void);
/* creates an empty drop */
Drop create_empty_drop(void);
/* deletes all current drops */
void reset_drops();

void game_over_screen(void);
void game_won(void);
void level_won(void);
int is_level_won(void);
void load_next_bricks(const int level[MAX_ROWS][MAX_COLS]);

#endif /* !LOGIC_H_ */
