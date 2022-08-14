#ifndef EVENT_H_
#define EVENT_H_

extern App app;

/* handles events and start over event handlers */
void handle_events(void);
/* board event handler */
void handle_board_event(SDL_Event event);
/* ball event handler */
void handle_ball_event(SDL_Event event);

#endif /* !EVENT_H_ */
