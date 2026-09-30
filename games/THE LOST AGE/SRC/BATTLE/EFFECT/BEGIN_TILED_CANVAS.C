#include "TYPES.H"

/* Battle effect: open the canvas layer with a 16 by 16 tile canvas on a
   32-wide map. The left half of each map row holds the canvas tile pairs;
   where ☀️ clears the right half, ⚓️ fills it with tile 0x0f pairs and sets
   a smaller screen size. */

void BattleFx_BeginCanvasLayer(s32 bg_control);

void BattleFx_BeginTiledCanvas(s32 bg_control)
{
    s32 row;
    s32 col;
    s32 offset;

    BattleFx_BeginCanvasLayer(bg_control);
    *(volatile u16 *)0x0400000c = bg_control | 0x4784;
    offset = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++, offset += 2) {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)(0x06003800 + offset) = pair;
        }
        for (col = 0; col != 8; col++, offset += 2) {
            *(volatile u16 *)(0x06003800 + offset) = 0xf0f;
        }
    }
}
