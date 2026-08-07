#include "types.h"

/*
 * Resource 394 piece spawner at 0x02000a90.
 *
 * Complete owner: `push {r5, r6, lr}` plus `mov r6,r8 / push {r6}` at
 * 0x02000a90, and the matching `pop {r3} / mov r8,r3 / pop {r5, r6} /
 * pop {r0} / bx r0` at 0x02000b32.  172-byte row, all code — no literal pool:
 * every constant is built with `movs`/`lsls`, and the next owner's prologue is
 * at 0x02000b3c.  The return address is popped into r0, so the owner is
 * `void`.
 *
 * Called once, from the root 0x02000980, with the record list pointer held in
 * the cell at 0x020092c0.
 *
 * The record list is the same twelve-byte-stride, -1-terminated array that
 * 0x02000b3c stamps: an s16 id at +0, column at +2, row at +4, an orientation
 * flag at +6, and the spawned object pointer written back at +8.
 *
 * Tile-to-16.16 conversion: `<< 20` is the tile-grid idiom read backwards
 * (`<< 4` for the 16-pixel grid, `<< 16` to fixed point).  The half-tile bias
 * added to each axis is 0x200000 along the piece's long axis and 0x80000
 * across it, which is what makes the orientation flag swap the two constants
 * rather than change anything else.
 *
 * Four call sites, matching the row's advertised count.  The two arms of the
 * orientation test are kept separate even though they differ only in which
 * constant goes to which axis: merging them would not change this row's
 * multiset, but it would hide that the assembly really does branch.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
u8 *Func_02001b24();
void Func_02001b2a();
void Func_02001b7a();
s32 Func_02001b7c();
                       /* spawn an object: (id, x, unused, z) */
                       /* set object motion state */
                       /* attach the object to its display slot */
                       /* height of the board at (x, z) */

void Func_02000a90(s16 *records)
{
    s16 *record = records;
    u16 id = (u16)record[0];

    if (record[0] == -1) return;

    do {
        s32 x;
        s32 z;
        u8 *piece;
        s32 lift;

        if (record[3] == 0) {
            x = (record[1] << 20) + 0x200000;
            z = (record[2] << 20) + 0x80000;
        } else {
            x = (record[1] << 20) + 0x80000;
            z = (record[2] << 20) + 0x200000;
        }

        piece = Func_02001b24((s32)(s16)id, x, 0, z);
        if (piece == 0) return;

        *(u8 **)(record + 4) = piece;   /* the pointer field at +8 */
        Func_02001b2a(piece, 1);
        Func_02001b7a(piece, 0);

        piece[89] = 0;
        *(u16 *)(piece + 32) = 32;

        lift = Func_02001b7c(0, *(s16 *)(piece + 10), *(s16 *)(piece + 18));

        *(s32 *)(piece + 12) += lift << 16;
        record += 6;
        *(s32 *)(piece + 20) = lift << 16;

        id = (u16)record[0];
    } while ((s16)id != -1);
}
