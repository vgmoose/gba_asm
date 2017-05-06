
#include <stdint.h>

// change these to size of the screen:
#define screen_x 240
#define screen_y 160

extern uint8_t * screen; // bgr

void screen_init(const char * title);
void screen_end();
void screen_clear();
void screen_flip();

void * render_text(char * text, char r, char g, char b);
void draw_text(int x, int y, void * render);

void draw_pixel(int x, int y, char r, char g, char b);
void draw_line(int x0, int y0, int x1, int y1, char r, char g, char b);
void draw_rect(int x1, int y1, int x2, int y2, char r, char g, char b);
void draw_fill_rect(int x1, int y1, int x2, int y2, char r, char g, char b);
void draw_circle(int xCen, int yCen, int radius, char r, char g, char b);
void draw_fill_circle(int xCen, int yCen, int radius, char r, char g, char b);
void draw_circle_circum(int cx, int cy, int x, int y, char r, char g, char b);
