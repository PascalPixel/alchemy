/* Draft, main:0801f088, 376 bytes including its literal pool.
 * Whole owner split from 0801ef68; render one five-tile status bar by
 * replacing palette indices 14 and 1 in one or three tile rows.
 * First typed model: 356 bytes, 99 aligned halfword edits. A named VRAM
 * array gives 360 bytes / 94 edits; retained below. A separate byte offset
 * alongside the row counter gives 360 / 106 and was not retained.
 * Remaining: reference frame 20 versus 12 bytes; it keeps the VRAM base
 * in fp and the tile id in sl, while x and the outer counter spill. Ours
 * strength-reduces the destination walk and keeps the outer locals live.
 * Stop this spelling axis; inspect loop/allocation dumps before another
 * attempt. Neither the palette setup nor the pixel loop is missing.
 */
#include "TYPES.H"
#include "RENDER_INPUT.H"
#include "DMA.H"

extern u8 *gWindowWork;
extern u32 Data_0600001c[];
s32 Runtime_GetLowTableAddress(void);

/* FAKEMATCH: leave the last pixel value live in r0 at the return. */
s32 UiWindow_DrawStatusBarTiles(struct RenderInput *window, s32 x, s32 y, s32 value)
{
    u8 *base = gWindowWork;
    s32 filled = value;
    s32 tile;

    if (base[0xea5] == 0) {
        Dma_Set((const void *)Runtime_GetLowTableAddress(), (void *)0x050001c0,
            0x80000010, (volatile u32 *)0x040000d4);
        *(volatile u16 *)0x050001dc = *(volatile u16 *)0x050001e8;
    }
    x += window->x;
    y += window->y;
    for (tile = 4; tile >= 0; tile--, x++, value -= 8) {
        u32 id = ((u16 *)base)[y * 32 + x] & 0x3ff;
        u32 light = 0x22222222;
        u32 dark = 0xcccccccc;
        s32 row;

        if (value > 7) {
            light = 0x88888888;
            dark = 0xdddddddd;
        } else if (value >= 0) {
            u32 shift = value * 4;
            /* FAKEMATCH: target variable shifts also handle the zero-width edge. */
            light = (light << shift) | (0x88888888U >> (32 - shift));
            dark = (dark << shift) | (0xddddddddU >> (32 - shift));
        }
        for (row = 0; row <= (filled != 0 ? 2 : 0); row++) {
            u32 *dest = &Data_0600001c[id * 8 - row];
            u32 pixels = *dest;
            u32 result = 0;
            s32 col;
            for (col = 0; col <= 7; col++) {
                u32 color = pixels & 15;
                if (color == 14)
                    result |= light & (15U << (col * 4));
                else if (color == 1)
                    result |= dark & (15U << (col * 4));
                else
                    result |= color << (col * 4);
                pixels >>= 4;
            }
            *dest = result;
        }
    }
}
