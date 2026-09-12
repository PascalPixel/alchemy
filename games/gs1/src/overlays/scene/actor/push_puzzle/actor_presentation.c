#include "types.h"

#define ActorPresentation_RepaintTenCellsAndActorEightCell Func_02000cb4
extern u8 Data_02009d28[];

u8 *Func_02001d90();
void Func_02001d5c(s32, s32, s32, s32, s32, s32);
void Func_02001d74(s32, s32, s32, s32, s32, s32);

/*
 * Ten (x, z) tile pairs, held in the overlay's own writable image.  Overlay
 * data lives in EWRAM and is deliberately not const.
 */

/*
 * Slot accessor: Func_02001d90(slot) returns the actor record, or NULL.
 * Typed as a byte pointer so the +0x08 and +0x10 field reads are explicit.
 */

/*
 * The six-argument renderer ABI: four register arguments plus two stack
 * words, here the tile x and tile z of the cell being repainted.  The two
 * names are separate per-site call words that reach the same renderer.
 */

/*
 * Repaint ten fixed collision cells and then actor 8's own cell.  The
 * 92-byte owner includes the alignment halfword and the single pool word
 * that follows the code; that word holds the address of Data_02009d28, which
 * is in-image data rather than a RAM global.  The two renderer calls must
 * keep their separate call words -- naming one renderer for both changes the
 * displacement emitted at each site.
 */
void ActorPresentation_RepaintTenCellsAndActorEightCell(void)
{
    u8 *actor;
    s32 tx;
    s32 tz;
    u32 i;

    /*
     * Slot 8 is the scene's own actor; the accessor result is not
     * null-checked here.  The 20-bit shift is one signed arithmetic shift:
     * 16 takes the fixed-point coordinate to pixels, the further 4 take it
     * to the 16-pixel tile grid.
     */
    actor = Func_02001d90(8);
    tx = *(s32 *)(actor + 0x08) >> 20;
    tz = *(s32 *)(actor + 0x10) >> 20;

    /*
     * Ten fixed cells from the table, then the actor's own cell.  The table
     * is walked by the byte index itself rather than by a 0..9 counter
     * scaled by two, so the loop steps the byte offset directly.
     */
    for (i = 0; i < 20; i += 2) {
        s32 x = (s32)Data_02009d28[i];
        s32 z = (s32)Data_02009d28[i + 1];
        Func_02001d5c(1, 0, 1, 1, x, z);
    }

    /*
     * The same repaint with 0 rather than 1 in the first argument.  What
     * that selector chooses is not established.
     */
    Func_02001d74(0, 0, 1, 1, tx, tz);
}
