#ifndef EVENT_H_
#define EVENT_H_

extern App app;

void handle_events(void);
void handle_board_event(SDL_Event event);

#endif /* !EVENT_H_ */
