#include "types.h"

void Func_080cd594(void);

/*
 * Build the 16x16 affine-background map at screen block 7.  The left half
 * of each row selects progressively offset tiles and palette banks; the
 * right half is deliberately cleared.  Callers choose the display-control
 * variant through the low bits of `mode`.
 */
void Func_080cdb24(s32 mode)
{
    volatile u16 *display_control = (volatile u16 *)0x0400000c;
    volatile u16 *screen = (volatile u16 *)0x06003800;
    u32 row;

    Func_080cd594();
    *display_control = (u16)(mode | 0x6784);

    for (row = 0; row < 16; row++) {
        u32 column;
        u16 tile = (u16)(0x0100 + row * 0x1000);
        u16 attribute = (u16)(row * 16);

        for (column = 0; column < 8; column++) {
            screen[row * 16 + column] = (u16)(tile | attribute);
            tile += 0x0200;
            attribute += 2;
        }
        for (; column < 16; column++)
            screen[row * 16 + column] = 0;
    }
}
