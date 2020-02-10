
#include <stdint.h>

// change these to size of the screen:
#define screen_x 240
#define screen_y 160

extern uint16_t screen[screen_x * screen_y]; // bgr555

char screen_init(const char * title);
void screen_end();
void screen_clear();
void screen_flip();

void * render_text(char * text, char r, char g, char b);
void draw_text(int x, int y, void * render);

void draw_pixel(int x, int y, uint16_t g);
