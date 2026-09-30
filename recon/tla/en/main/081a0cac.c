#include "TYPES.H"

extern u16 Data_02004c00;
extern s16 Flash_Handler0;
extern u32 *gFlashNumRemainingBytes;
extern u32 gFrameTick;

/* Lay the 16 x 6 grid of wide text objects over the scroll position: each
   row is eight pixels lower less the position's fine offset, and the tiles
   continue from the position's line in the 0x300-tile ring. Copy the table
   to OAM, then advance the position every fourth frame unless a line is
   still being drawn. */

void DisplayScroll_UpdateObjects(void)
{
    u32 frame = Data_02004c00;
    u32 fine = frame & 7;
    s32 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
    u32 *entry = gFlashNumRemainingBytes + 48;
    s32 row;

    for (row = 0; row <= 15; row++) {
        s32 col;

        for (col = 0; col < 6; col++) {
            u32 *q = entry;

            *q++ = (row * 8 + 16 - fine) | ((col * 32 + 24) << 16) | 0x40004000;
            *q = tile;
            tile += 4;
            entry += 2;
            if (tile == 0x300)
                tile = 0;
        }
    }
    Dma_Set(gFlashNumRemainingBytes, (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
    if (Flash_Handler0 == 0 && (gFrameTick & 3) == 0)
        Data_02004c00++;
}
