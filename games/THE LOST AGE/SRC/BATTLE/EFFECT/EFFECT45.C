#include "TYPES.H"

void BattleFx_BeginCanvasLayer(s32 bg_control);

/* Battle effect: open the canvas layer with a 16 by 16 tile canvas on a
   32-wide map. The left half of each map row holds the canvas tile pairs,
   the right half is cleared. ⚓️ sets a smaller screen size than ☀️. */
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
            *(volatile u16 *)(0x06003800 + offset) = 0;
        }
    }
}

/* ⚓️'s second canvas: the same tile pairs, with the right half of each map
   row filled with tile 0x0f pairs instead of cleared. */
void BattleFx_BeginTiledCanvasFilled(s32 bg_control)
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
