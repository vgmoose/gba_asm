// MAIN.C
// SNAKE 1.0
// JUNE 1, 2009

#include <stdio.h>
#include "snakebmp.h"
#include "ascii_font.h"
#include "soundtrack.h"

#define BUTTON_A		(!((*(volatile unsigned int*)0x04000130)&1))
#define BUTTON_B		(!((*(volatile unsigned int*)0x04000130)&2))
#define BUTTON_SELECT	(!((*(volatile unsigned int*)0x04000130)&4))
#define BUTTON_START	(!((*(volatile unsigned int*)0x04000130)&8))
#define BUTTON_RIGHT	(!((*(volatile unsigned int*)0x04000130)&16))
#define BUTTON_LEFT	(!((*(volatile unsigned int*)0x04000130)&32))
#define BUTTON_UP		(!((*(volatile unsigned int*)0x04000130)&64))
#define BUTTON_DOWN	(!((*(volatile unsigned int*)0x04000130)&128))
#define BUTTON_R		(!((*(volatile unsigned int*)0x04000130)&256))
#define BUTTON_L		(!((*(volatile unsigned int*)0x04000130)&512))

#define RIGHT 16
#define LEFT 32
#define UP 64
#define DOWN 128

#define GRASS 0
#define FOOD 1
#define TAIL_R 2
#define TAIL_U 3
#define TAIL_L 4
#define TAIL_D 5
#define HEAD_R 6
#define HEAD_U 7
#define HEAD_L 8
#define HEAD_D 9
#define BODY_LR 10
#define BODY_LD 11
#define BODY_RD 12
#define BODY_RU 13
#define BODY_LU 14
#define BODY_UD 15
#define WALL 16

#define WHITE 0xFFFF
#define RED 0x00FF
#define BLUE 0xEE00
#define CYAN 0xFF00
#define GREEN 0x03E0
#define MAGENTA 0xF00F
#define BROWN 0x0D0D
#define BLACK 0x0000
#define BRICK 0x011B

unsigned int seed;

const int start_screen [10][15] =
{{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
{BODY_RD, BODY_LR, TAIL_L, BODY_RD, BODY_LR, BODY_LD, BODY_RD, BODY_LR, BODY_LD, HEAD_U, GRASS, GRASS, BODY_RD, BODY_LR, HEAD_R},
{BODY_UD, GRASS, GRASS, BODY_UD, GRASS, BODY_UD, BODY_UD, GRASS, BODY_UD, BODY_UD, GRASS, HEAD_U, BODY_UD, GRASS, GRASS},
{BODY_RU, BODY_LR, BODY_LD, BODY_UD, GRASS, BODY_UD, BODY_UD, FOOD, BODY_UD, BODY_UD, BODY_RD, BODY_LU, BODY_UD, TAIL_R, HEAD_R},
{GRASS, GRASS, BODY_UD, BODY_UD, GRASS, BODY_UD, BODY_UD, GRASS, BODY_UD, BODY_UD, BODY_RU, BODY_LD, BODY_UD, GRASS, GRASS},
{HEAD_L, BODY_LR, BODY_LU, TAIL_U, GRASS, HEAD_D, TAIL_U, GRASS, HEAD_D, TAIL_U, GRASS, TAIL_U, BODY_RU, BODY_LR, TAIL_L},
{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS},
{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
{GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS}};

int board[15][10];

void print_bitmap(int x, int y, int n);
void place_food(void);
unsigned int ayn(void);

unsigned int ayn(void)
{
    seed = seed * 69069 + 1;
    return (seed >> 16);
}
    
void print_bitmap(int x, int y, int n)
{
    // create a pointer to the video buffer
    unsigned short* videoBuffer = (unsigned short*)0x6000000;
    
    int i, j;

    for(i=0; i<16; i++)
    for(j=0; j<16; j++)
       videoBuffer[(16*y + j)*240 + (16*x + i)]
          = snakebmp[2*(256*n+16*j + i)] + 256*snakebmp[2*(256*n+16*j + i) + 1];
}

void place_food(void)
{
    int x, y;
    
one_more_try:

    x=1+ayn()%13;
    y=1+ayn()%8;

    if (board[x][y]==GRASS)
       {
        board[x][y] = FOOD;
        print_bitmap(x,y,FOOD);
        return;
        }
    else
        goto one_more_try;
}

int main(void)
{
int x, y, i, button, in_front, score;
int xtail, xhead, ytail, yhead;
char buffer [11];

// TIMER 1

*(volatile unsigned short *) 0x04000104 = 0;
// REG_TM01 = 0;

*(volatile unsigned short *) 0x04000106 = 0x0080;
// REG_TM1CNT = TIMER_ENABLE;   // = 0x0080

StartSound();

// switch to video mode 3 (240x160 16-bit) and background 2
// by setting a memory register to a specific value
*(unsigned long*)0x4000000 = 0x403;

for (x=0; x<15; x++)
for (y=0; y<10; y++)
    print_bitmap(x, y, start_screen[y][x]);
    
Print( 0, 148, "START=NEW GAME  SELECT=CREDITS", BLACK, GREEN );

while(1)
{
   printf("Start: %d\n", BUTTON_START);
   printf("Down: %d\n", BUTTON_DOWN);

   //if (BUTTON_START) goto start_game;
   //if (BUTTON_SELECT) goto show_credits;
}

start_game:

// INITIALIZE THE RANDOM NUMBER GENERATOR USING TIMER 1

seed = *(volatile unsigned short *) 0x04000104;

xtail = 4;
ytail = 5;
xhead = 6;
yhead = 5;

button = RIGHT;
in_front = GRASS;

for (x=0; x<15; x++) board[x][0]=WALL;
for (x=0; x<15; x++) board[x][9]=WALL;
for (y=1; y<9; y++) board[0][y]=WALL;
for (y=1; y<9; y++) board[14][y]=WALL;

for (x=1; x<14; x++)
for (y=1; y<9; y++)
   board[x][y]=GRASS;

board[4][5]=TAIL_R;
board[5][5]=BODY_LR;
board[6][5]=HEAD_R;

for (x=0; x<15; x++)
for (y=0; y<10; y++)
    print_bitmap(x, y, board[x][y]);

place_food();

score = 0;

Print( 80, 4, "SCORE = 0", BLACK, BRICK);
    
move_snake:
for(i=0;i<25000;i=i+1)
{
   if (BUTTON_RIGHT) button=RIGHT;
   if (BUTTON_LEFT) button=LEFT;
   if (BUTTON_UP) button=UP;
   if (BUTTON_DOWN) button=DOWN;
}

switch (board[xhead][yhead])
{
case HEAD_R:
    switch (button)
    {
    case UP:
        in_front = board[xhead][yhead-1];
        if(in_front>1) goto head_r_default;
        board[xhead][yhead] = BODY_LU;
        print_bitmap(xhead,yhead,BODY_LU);
        board[xhead][yhead-1] = HEAD_U;
        print_bitmap(xhead,yhead-1,HEAD_U);
        yhead=yhead-1;
        break;
    case DOWN:
        in_front = board[xhead][yhead+1];
        if(in_front>1) goto head_r_default;
        board[xhead][yhead] = BODY_LD;
        print_bitmap(xhead,yhead,BODY_LD);
        board[xhead][yhead+1] = HEAD_D;
        print_bitmap(xhead,yhead+1,HEAD_D);
        yhead=yhead+1;
        break;
    default:
    head_r_default:
        in_front = board[xhead+1][yhead];
        if(in_front>1) goto stop_game;
        board[xhead][yhead] = BODY_LR;
        print_bitmap(xhead,yhead,BODY_LR);
        board[xhead+1][yhead] = HEAD_R;
        print_bitmap(xhead+1,yhead,HEAD_R);
        xhead=xhead+1;
    }
    break;
case HEAD_L:
    switch (button)
    {
    case UP:
        in_front = board[xhead][yhead-1];
        if(in_front>1) goto head_l_default;
        board[xhead][yhead] = BODY_RU;
        print_bitmap(xhead,yhead,BODY_RU);
        board[xhead][yhead-1] = HEAD_U;
        print_bitmap(xhead,yhead-1,HEAD_U);
        yhead=yhead-1;
        break;
    case DOWN:
        in_front = board[xhead][yhead+1];
        if(in_front>1) goto head_l_default;
        board[xhead][yhead] = BODY_RD;
        print_bitmap(xhead,yhead,BODY_RD);
        board[xhead][yhead+1] = HEAD_D;
        print_bitmap(xhead,yhead+1,HEAD_D);
        yhead=yhead+1;
        break;
    default:
    head_l_default:
        in_front = board[xhead-1][yhead];
        if(in_front>1) goto stop_game;
        board[xhead][yhead] = BODY_LR;
        print_bitmap(xhead,yhead,BODY_LR);
        board[xhead-1][yhead] = HEAD_L;
        print_bitmap(xhead-1,yhead,HEAD_L);
        xhead=xhead-1;
    }
    break;
case HEAD_U:
    switch (button)
    {
    case LEFT:
        in_front = board[xhead-1][yhead];
        if(in_front>1) goto head_u_default;
        board[xhead][yhead] = BODY_LD;
        print_bitmap(xhead,yhead,BODY_LD);
        board[xhead-1][yhead] = HEAD_L;
        print_bitmap(xhead-1,yhead,HEAD_L);
        xhead=xhead-1;
        break;
    case RIGHT:
        in_front = board[xhead+1][yhead];
        if(in_front>1) goto head_u_default;
        board[xhead][yhead] = BODY_RD;
        print_bitmap(xhead,yhead,BODY_RD);
        board[xhead+1][yhead] = HEAD_R;
        print_bitmap(xhead+1,yhead,HEAD_R);
        xhead=xhead+1;
        break;
    default:
    head_u_default:
        in_front = board[xhead][yhead-1];
        if(in_front>1) goto stop_game;
        board[xhead][yhead] = BODY_UD;
        print_bitmap(xhead,yhead,BODY_UD);
        board[xhead][yhead-1] = HEAD_U;
        print_bitmap(xhead,yhead-1,HEAD_U);
        yhead=yhead-1;
    }
    break;
case HEAD_D:
    switch (button)
    {
    case LEFT:
        in_front = board[xhead-1][yhead];
        if(in_front>1) goto head_d_default;
        board[xhead][yhead] = BODY_LU;
        print_bitmap(xhead,yhead,BODY_LU);
        board[xhead-1][yhead] = HEAD_L;
        print_bitmap(xhead-1,yhead,HEAD_L);
        xhead=xhead-1;
        break;
    case RIGHT:
        in_front = board[xhead+1][yhead];
        if(in_front>1) goto head_d_default;
        board[xhead][yhead] = BODY_RU;
        print_bitmap(xhead,yhead,BODY_RU);
        board[xhead+1][yhead] = HEAD_R;
        print_bitmap(xhead+1,yhead,HEAD_R);
        xhead=xhead+1;
        break;
    default:
    head_d_default:
        in_front = board[xhead][yhead+1];
        if(in_front>1) goto stop_game;
        board[xhead][yhead] = BODY_UD;
        print_bitmap(xhead,yhead,BODY_UD);
        board[xhead][yhead+1] = HEAD_D;
        print_bitmap(xhead,yhead+1,HEAD_D);
        yhead=yhead+1;
    }
    break;
}

if(in_front==FOOD)
    {
    score = score + 1;
    sprintf(buffer, "SCORE = %3d", score);
    Print( 80, 4, buffer, BLACK, BRICK);
    place_food();
    goto move_snake;
    }

switch (board[xtail][ytail])
{
case TAIL_R:
    switch (board[xtail+1][ytail])
    {
    case BODY_LR:
        board[xtail+1][ytail] = TAIL_R;
        print_bitmap(xtail+1,ytail,TAIL_R);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        xtail=xtail+1;
        break;
    case BODY_LU:
        board[xtail+1][ytail] = TAIL_U;
        print_bitmap(xtail+1,ytail,TAIL_U);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        xtail=xtail+1;
        break;
    case BODY_LD:
        board[xtail+1][ytail] = TAIL_D;
        print_bitmap(xtail+1,ytail,TAIL_D);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        xtail=xtail+1;
        break;
    }
    break;
case TAIL_L:
    switch (board[xtail-1][ytail])
    {
    case BODY_LR:
        board[xtail-1][ytail] = TAIL_L;
        print_bitmap(xtail-1,ytail,TAIL_L);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        xtail=xtail-1;
        break;
    case BODY_RU:
        board[xtail-1][ytail] = TAIL_U;
        print_bitmap(xtail-1,ytail,TAIL_U);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        xtail=xtail-1;
        break;
    case BODY_RD:
        board[xtail-1][ytail] = TAIL_D;
        print_bitmap(xtail-1,ytail,TAIL_D);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        xtail=xtail-1;
        break;
    }
    break;
case TAIL_U:
    switch (board[xtail][ytail-1])
    {
    case BODY_UD:
        board[xtail][ytail-1] = TAIL_U;
        print_bitmap(xtail,ytail-1,TAIL_U);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        ytail=ytail-1;
        break;
    case BODY_LD:
        board[xtail][ytail-1] = TAIL_L;
        print_bitmap(xtail,ytail-1,TAIL_L);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        ytail=ytail-1;
        break;
    case BODY_RD:
        board[xtail][ytail-1] = TAIL_R;
        print_bitmap(xtail,ytail-1,TAIL_R);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        ytail=ytail-1;
        break;
    }
    break;
case TAIL_D:
    switch (board[xtail][ytail+1])
    {
    case BODY_UD:
        board[xtail][ytail+1] = TAIL_D;
        print_bitmap(xtail,ytail+1,TAIL_D);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        ytail=ytail+1;
        break;
    case BODY_LU:
        board[xtail][ytail+1] = TAIL_L;
        print_bitmap(xtail,ytail+1,TAIL_L);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        ytail=ytail+1;
        break;
    case BODY_RU:
        board[xtail][ytail+1] = TAIL_R;
        print_bitmap(xtail,ytail+1,TAIL_R);
        board[xtail][ytail] = GRASS;
        print_bitmap(xtail,ytail,GRASS);
        ytail=ytail+1;
        break;
    }
    break;
}

goto move_snake;
    
show_credits:

for (x=0; x<15; x++)
for (y=0; y<10; y++)
    print_bitmap(x, y, GRASS);
    
Print( 0,2,  "          SNAKE 1.0", BLACK, GREEN);
Print( 0,12,  "2009(C) BAY PATH NINTENDO CLUB", BLACK, GREEN);
Print( 0,30,  "      DR. CALIN GALERIU", BLACK, GREEN);
Print( 0,40,  "      MR. SCOTT EDWARDS", BLACK, GREEN);
Print( 0,60,  "        JASON LAFORCE", BLACK, GREEN);
Print( 0,70,  "       ALEXANDER NEWTON", BLACK, GREEN);
Print( 0,80,  "         RYAN CUSSON", BLACK, GREEN);
Print( 0,90,  "          RYAN BERRY", BLACK, GREEN);
Print( 0,110,  "  BITMAPPED FONT COURTESY OF", BLACK, GREEN);
Print( 0,120,  "    PROF. JONATHAN HARBOUR", BLACK, GREEN);
Print( 0,140,  "MUSIC COMPOSED & PERFORMED BY", BLACK, GREEN);
Print( 0,150,  "STEPHAN ZEVESKA & JOSHUA WHITE", BLACK, GREEN);

stop_game:

while(1)
{
   if (BUTTON_START) goto start_game;
   if (BUTTON_SELECT) goto show_credits;
}

}







