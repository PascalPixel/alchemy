/* resource_3b4 actor presentation: cell repaints for slots 11 and 12. */
#include "types.h"

/*
 * Func_ names below are loader-relocated call words in this overlay's import
 * veneer table, not runtime addresses.  The declarations are old-style
 * because the same imports are reached with differing argument counts from
 * different call sites.
 */

void Func_02003b9e();
s32 *Func_02004344();
void Func_020042fa(s32, s32, s32, s32, s32, s32);
s32 *Func_02004368();
void Func_0200431c(s32, s32, s32, s32, s32, s32);

/*
 * Repaint the four cells, then one cell for each of slots 11 and 12 at that
 * slot's own X tile.  The 92-byte owner at 0x02001df8 includes two bytes of
 * alignment and the pool word 0x000fffff.  The tile divisions must stay
 * spelled `/ 0x100000': the reference biases a negative value before the
 * arithmetic shift, which is exactly this truncating signed division.
 */

void ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void)
{
    s32 *slot;
    s32 tile;

    Func_02003b9e();

    slot = Func_02004344(11);
    tile = slot[2] / 0x100000;
    Func_020042fa(53, 55, 1, 1, tile, 55);
    slot = Func_02004368(12);
    tile = slot[2] / 0x100000;
    Func_0200431c(53, 55, 1, 1, tile, 55);
}
