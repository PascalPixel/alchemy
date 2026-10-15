/* Draft, differ 8 of 65: as Func_08164abc from the battle work's saved
 * palette (BattleWork.palette) into BG palette entries 96 to 223. The loop
 * matches; the destination set first takes the prologue from 12 to 8.
 * Left is the prologue's order: the native starts 0x03000000, copies the
 * first and third deltas, loads the battle work, starts 0x56c, loads the
 * pooled 0x1f mask, then starts 0x050000c0; here the first delta is
 * copied between the halves of 0x03000000 and the mask loads later.
 * Worked through sched2's ranking by hand, the native's whole prologue
 * order follows if a load costs 3 cycles instead of agscc's 2 (the work's
 * load then outranks the first copy and the mask outranks 0x050000c0's
 * shift); the same holds for Func_08164a4c. Tried: the work's load or the
 * source in a one-pass block (22, 24), alchemy permute 5 minutes (no
 * candidate below 8). */
#include "types.h"
#include "battle_work.h"

void Func_08164b2c(s32 redDelta, s32 greenDelta, s32 blueDelta)
{
    u16 *color = (u16 *)0x050000c0;
    u16 *source = Ram_HeapSlots->battleWork->palette;
    s32 i;

    for (i = 0; i != 128; i++) {
        s32 blue = (*source >> 10) & 0x1f;
        s32 green = (*source >> 5) & 0x1f;
        s32 red = *source & 0x1f;

        red += redDelta;
        green += greenDelta;
        blue += blueDelta;
        if (red > 31)
            red = 31;
        if (green > 31)
            green = 31;
        if (blue > 31)
            blue = 31;
        if (red < 0)
            red = 0;
        if (green < 0)
            green = 0;
        if (blue < 0)
            blue = 0;
        *color++ = (blue << 10) | (green << 5) | red;
        source++;
    }
}
