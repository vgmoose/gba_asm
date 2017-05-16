#include "toolbox.h"
#include "../1up/1up.h"

#define uint unsigned int

//! Draw a line on a 16bpp canvas
void bmp16_line(int x1, int y1, int x2, int y2, u32 clr,
    void *dstBase, uint dstPitch)
{
    int ii, dx, dy, xstep, ystep, dd;
    u16 *dst= (u16*)(dstBase + y1*dstPitch + x1*2);
    dstPitch /= 2;

    // --- Normalization ---
    if(x1>x2)
    {   xstep= -1;  dx= x1-x2;  }
    else
    {   xstep= +1;  dx= x2-x1;  }

    if(y1>y2)
    {   ystep= -dstPitch;   dy= y1-y2;  }
    else
    {   ystep= +dstPitch;   dy= y2-y1;  }


    // --- Drawing ---

    if(dy == 0)         // Horizontal
    {
        for(ii=0; ii<=dx; ii++)
            dst[ii*xstep]= clr;
    }
    else if(dx == 0)    // Vertical
    {
        for(ii=0; ii<=dy; ii++)
            dst[ii*ystep]= clr;
    }
    else if(dx>=dy)     // Diagonal, slope <= 1
    {
        dd= 2*dy - dx;

        for(ii=0; ii<=dx; ii++)
        {
            *dst= clr;
            if(dd >= 0)
            {   dd -= 2*dx; dst += ystep;  }

            dd += 2*dy;
            dst += xstep;
        }               
    }
    else                // Diagonal, slope > 1
    {
        dd= 2*dx - dy;

        for(ii=0; ii<=dy; ii++)
        {
            *dst= clr;
            if(dd >= 0)
            {   dd -= 2*dy; dst += xstep;  }

            dd += 2*dx;
            dst += ystep;
        }       
    }
}

//! Draw a rectangle on a 16bpp canvas
void bmp16_rect(int left, int top, int right, int bottom, u32 clr,
    void *dstBase, uint dstPitch)
{
    int ix, iy;

    uint width= right-left, height= bottom-top;
    u16 *dst= (u16*)(dstBase+top*dstPitch + left*2);
    dstPitch /= 2;

    // --- Draw ---
    for(iy=0; iy<height; iy++)
        for(ix=0; ix<width; ix++)
            dst[iy*dstPitch + ix]= clr;
}

//! Draw a frame on a 16bpp canvas
void bmp16_frame(int left, int top, int right, int bottom, u32 clr,
    void *dstBase, uint dstPitch)
{
    // Frame is RB exclusive
    right--;
    bottom--;

    bmp16_line(left,  top,   right,  top,    clr, dstBase, dstPitch);
    bmp16_line(left,  bottom, right, bottom, clr, dstBase, dstPitch);

    bmp16_line(left,  top,   left,   bottom, clr, dstBase, dstPitch);
    bmp16_line(right, top,   right,  bottom, clr, dstBase, dstPitch);
}

typedef u16 COLOR;


#define M3_WIDTH        240


// === PROTOTYPES =====================================================

INLINE void m3_line(int x1, int y1, int x2, int y2, COLOR clr);
INLINE void m3_rect(int left, int top, int right, int bottom, COLOR clr);
INLINE void m3_frame(int left, int top, int right, int bottom, COLOR clr);


// === INLINES ========================================================

//! Plot a single \a clr colored pixel in mode 3 at (\a x, \a y).
INLINE void m3_plot(int x, int y, COLOR clr)
{
    vid_mem[y*M3_WIDTH+x]= clr; 
}

//! Draw a \a clr colored line in mode 3.
INLINE void m3_line(int x1, int y1, int x2, int y2, COLOR clr)
{
    bmp16_line(x1, y1, x2, y2, clr, vid_mem, M3_WIDTH*2);
}

//! Draw a \a clr colored rectangle in mode 3.
INLINE void m3_rect(int left, int top, int right, int bottom, COLOR clr)
{
    bmp16_rect(left, top, right, bottom, clr, vid_mem, M3_WIDTH*2);
}

//! Draw a \a clr colored frame in mode 3.
INLINE void m3_frame(int left, int top, int right, int bottom, COLOR clr)
{
    bmp16_frame(left, top, right, bottom, clr, vid_mem, M3_WIDTH*2);
}

//! Fill the mode 3 background with color \a clr.
void m3_fill(COLOR clr) 
{   
    int ii;
    u32 *dst= (u32*)vid_mem;
    u32 wd= (clr<<16) | clr;

    for(ii=0; ii<M3_SIZE/4; ii++)
        *dst++= wd;
}

int main()
{
    int ii, jj;

	init();

    REG_DISPCNT= DCNT_MODE3 | DCNT_BG2;

    // Fill screen with grey color
    m3_fill(RGB15(12, 12, 14));

    // Rectangles:
    m3_rect( 12,  8, 108,  72, CLR_RED);
    m3_rect(108, 72, 132,  88, CLR_LIME);
    m3_rect(132, 88, 228, 152, CLR_BLUE);

    // Rectangle frames
    m3_frame(132,  8, 228,  72, CLR_CYAN);
    m3_frame(109, 73, 131,  87, CLR_BLACK);
    m3_frame( 12, 88, 108, 152, CLR_YELLOW);

    // Lines in top right frame
    for(ii=0; ii<=8; ii++)
    {
        jj= 3*ii+7;
        m3_line(132+11*ii, 9, 226, 12+7*ii, RGB15(jj, 0, jj));
        m3_line(226-11*ii,70, 133, 69-7*ii, RGB15(jj, 0, jj));
    }

    // Lines in bottom left frame
    for(ii=0; ii<=8; ii++)
    {
        jj= 3*ii+7;
        m3_line(15+11*ii, 88, 104-11*ii, 150, RGB15(0, jj, jj));
    }

    while(1);

    return 0;
}
