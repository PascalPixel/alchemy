#include "types.h"
#include "dma.h"

struct DisplayScrollObject {
    u32 attr01;
    u32 attr2;
};

extern u16 DisplayScroll_Position;
extern struct DisplayScrollObject *DisplayScroll_Objects;
extern u32 gFrameTick;

void Func_081a0cac(void)
{
    u32 frame = DisplayScroll_Position;
    u32 fine = frame & 7;
    s32 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
    u32 *entry = (u32 *)(DisplayScroll_Objects + 24);
    s32 row;

    for (row = 0; row <= 15; row++) {
        s32 col;

        for (col = 0; col < 6; col++) {
            u32 *q = entry;

            *q++ = (row * 8 + 16 - fine) | ((col * 32 + 24) << 16) | 0x40004000;
            entry += 2;
            /* FAKEMATCH: a one-pass block keeps the entry step ahead of the tile store; as plain statements, in either order, the step falls behind the tile step (2 differ). */
            do {
            } while (0);
            *q = tile;
            tile += 4;
            if (tile == 0x300)
                tile = 0;
        }
    }
    Dma_Set(DisplayScroll_Objects, (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
    if (gFrameTick % 3 == 0)
        DisplayScroll_Position++;
}
