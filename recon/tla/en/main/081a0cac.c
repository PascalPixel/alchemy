/* Near miss: DisplayScroll_UpdateObjects, ☀️'s grid over ⚓️'s intro crawl
   cells, differ 2 under the ARM9TDMI schedule: the native steps the entry
   pointer (adds r0, #8) before storing the tile word, this draft after.
   ⚓️ advances the crawl every third frame with no busy check. Reordering
   the entry step and the tile store, indexed stores and a 200-step device
   search did not move it; the grid in 081a08c4 shares the same gap.
   Joins DISPLAY6.C on a match. */
#include "TYPES.H"
#include "DMA.H"

struct DisplayScrollObject {
    u32 attr01;
    u32 attr2;
};

extern u16 DisplayScroll_Position;
extern struct DisplayScrollObject *DisplayScroll_Objects;
extern u32 gFrameTick;

void DisplayScroll_UpdateObjects(void)
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
            *q = tile;
            tile += 4;
            entry += 2;
            if (tile == 0x300)
                tile = 0;
        }
    }
    Dma_Set(DisplayScroll_Objects, (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
    if (gFrameTick % 3 == 0)
        DisplayScroll_Position++;
}
