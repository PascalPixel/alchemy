/* Near miss: differ 2 of 52 (one reordered row). The native loads the tilemap's address before
 * it subtracts the row from 13 (movs r2, #13; ldr r5, =0x06006000; subs r2, r2, r3); the draft
 * subtracts first. The Broken Seal's source, with this game's heap slot for the timer. */
#include "types.h"
#include "ram_buffer.h"

void BattlePres_DrawTransitionRows(void)
{
    u32 i;
    s32 frame;
    s32 aligned;
    s32 tile;
    u32 row;
    u16 *tiles;

    frame = *(s32 *)Ram_HeapSlots->unknown_84[9];
    if (frame >= 0 && frame <= 79) {
        tile = (7 & frame) + 0xf081;
        if (frame >= 0) {
            aligned = frame;
        } else {
            aligned = frame + 7;
        }
        row = 13 - (aligned >> 3);
        i = 0;
        tiles = (u16 *)((row << 6) + 0x06006000);
        do {
            i++;
            *tiles = tile;
            tiles++;
        } while (i != 32);

        tile = tile | 0x800;
        aligned = frame;
        if (frame < 0) {
            aligned = aligned + 7;
        }
        row = (aligned >> 3) + 13;
        if (row <= 20) {
            i = 0;
            tiles = (u16 *)((row << 6) + 0x06006000);
            do {
                i++;
                *tiles = tile;
                tiles++;
            } while (i != 32);
        }
    }
}
