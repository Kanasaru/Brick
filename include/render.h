#ifndef RENDER_H_
#define RENDER_H_

extern App app;
extern Board board;
extern Ball ball;

void prepareScene(void);
void presentScene(void);
void draw(void);
void draw_current_bricks(void);
void draw_brick(Brick br);
void draw_board(SDL_Renderer *renderer);
void draw_panel(SDL_Renderer *renderer, 
                int fps, 
                float lgcrt, 
                int lvl, 
                int pts);
void draw_ball(SDL_Renderer *renderer, int hitbox);
void draw_ball_hitbox(SDL_Renderer *renderer);

#endif /* !RENDER_H_ */
