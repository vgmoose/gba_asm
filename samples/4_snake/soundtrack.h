// SOUNDTRACK.C
// SNAKE 1.0
// JUNE 1, 2009

#include "snake8.h"

void LoopSound(void) __attribute__ ((section(".iwram")));
void StartSound(void);

int counter;

void LoopSound(void)
{
*(volatile unsigned short *) 0x04000208 = 0x00;
// REG_IME = 0;
    
counter = counter + 1;

if(counter == 1308592)
    {
    *(volatile unsigned short *) 0x04000102 = 0x00;
    // REG_TM0CNT = TIMER_DISABLE;   // = 0x00

    *(volatile unsigned short *) 0x040000C6 = 0x00;
    // REG_DM1CNT_H = DMA_DISABLE;   // = 0x00
    
    counter = 0;

    *(volatile unsigned long *) 0x040000BC = (unsigned long) snake8;
    // REG_DM1SAD = (u32) snake8;

    *(volatile unsigned short *) 0x040000C6 = 0xB640;
    // REG_DM1CNT_H = DMA_DEST_FIXED | DMA_REPEAT | DMA_WORD | DMA_MODE_FIFO | DMA_ENABLE;   // = 0xB640

    *(volatile unsigned short *) 0x04000102 = 0x00C0;
    // REG_TM0CNT = TIMER_ENABLE | IRQ;   // = 0x00C0
    }

*(volatile unsigned short *) 0x04000202 = 0x0008;
// REG_IF = IRQ_TIMER0;

*(volatile unsigned short *) 0x04000208 = 0x01;
// REG_IME = 1;
}

void StartSound(void)
{
counter = 0;
    
// DIRECT SOUND A

*(volatile unsigned short *) 0x04000082 = 0x0B04;
// REG_SOUNDCNT_H = SOUNDA_VOLUME_100 | SOUNDA_LOUT | SOUNDA_ROUT | timer 0 | SOUNDA_FIFORESET;   // = 0x0B04

*(volatile unsigned short *) 0x04000084 = 0x0080;
// REG_SOUNDCNT_X = SOUND_ENABLE;   // = 0x80

// DMA 1

*(volatile unsigned long *) 0x040000BC = (unsigned long) snake8;
// REG_DM1SAD = (u32) snake8;

*(volatile unsigned long *) 0x040000C0 = 0x040000A0;
// REG_DM1DAD = (u32) &(REG_SGFIFOA);

*(volatile unsigned short *) 0x040000C6 = 0xB640;
// REG_DM1CNT_H = DMA_DEST_FIXED | DMA_REPEAT | DMA_WORD | DMA_MODE_FIFO | DMA_ENABLE;   // = 0xB640

// INTERRUPT

*(volatile unsigned short *) 0x04000208 = 0x00;
// REG_IME = 0;

*(volatile unsigned long *) 0x03007FFC = (unsigned long) LoopSound;

*(volatile unsigned short *) 0x04000200 = 0x0008;
// REG_IE = IRQ_TIMER0;

*(volatile unsigned short *) 0x04000208 = 0x01;
// REG_IME = 1;

// TIMER 0

*(volatile unsigned short *) 0x04000100 = 65536 - (16777216 / 22050);
// REG_TM0D = 65536 - (16777216 / frequency);
// play sample every 761 CPU cycles

*(volatile unsigned short *) 0x04000102 = 0x00C0;
// REG_TM0CNT = TIMER_ENABLE | IRQ;   // = 0x00C0

}


