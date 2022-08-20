#ifndef INIT_H_
#define INIT_H_

extern App app;

/* inits SDL libary */
void init_SDL(void);
/* inits the game */
void init_game(void);
/* clears init */
void clr_init(void);
/* loads images, music and sounds */
void load_media(void);

#endif /* !INIT_H_ */
