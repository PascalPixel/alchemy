#include "types.h"

/*
 * Resource 394 metatile blit at 0x02000098.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the two high-register saves
 * (`mov r7,fp / mov r6,sl / mov r5,r9 / push {r5, r6, r7}` and
 * `mov r7,r8 / push {r7}`) at 0x02000098, and the matching unwind
 * `add sp,#8 / pop {r3, r5, r6, r7} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * mov fp,r7 / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000126.  184-byte
 * row: 160 bytes of code plus the six pool words at
 * 0x02000138-0x0200014f.  Control-flow walk: the two loop back-edges target
 * 0x020000ee and 0x020000ce, both well below the pool.  The return address is
 * popped into r0, so the owner is `void`.
 *
 * SEVEN arguments.  The prologue pushes eight registers and then `sub sp,#8`,
 * so the incoming stack arguments are at sp+0x28, sp+0x2c and sp+0x30 — the
 * fifth, sixth and seventh.  Reading only sp+0x28/sp+0x2c would silently drop
 * the destination row, which is the register the outer loop counts on.
 *
 * The map is the 0x02010000 cell array the sibling rows 0x02000b3c and
 * 0x02000b8c also address: 128 four-byte cells per row (`(y << 7) + x`, then
 * `<< 2`).  0x02010000 is below the 0x02008000 link band, so it is a plain
 * EWRAM address rather than an in-image offset.
 *
 * Each source cell contributes a pair of words eight bytes apart in the table
 * at 0x02020000, written to 0x06002800 and 0x06002840 — one map row apart in
 * VRAM.  That is the 2x2 metatile expansion; `& 15` on both counters wraps the
 * destination inside the 16-cell VRAM window and `bank << 4` selects which
 * 16-row block of it is written.
 *
 * No call sites, matching the row's advertised count of zero.
 */

void Func_02000098(s32 srcX, s32 srcY, s32 columns, s32 rows,
                   s32 bank, s32 dstX, s32 dstY)
{
    u32 *cell = (u32 *)(0x02010000 + (((srcY << 7) + srcX) << 2));
    s32 rowAdvance = (128 - columns) << 2;
    s32 lastRow = dstY + rows;
    s32 y;
    s32 x;

    if (dstY >= lastRow) return;

    for (y = dstY; y < lastRow; y++) {
        s32 lastColumn = dstX + columns;
        s32 rowBase = ((y & 15) + (bank << 4)) << 5;

        for (x = dstX; x < lastColumn; x++) {
            u32 entry = *cell++ & 0xfff;
            s32 offset = (rowBase + (x & 15)) << 2;

            *(u32 *)(0x06002800 + offset) = *(u32 *)(0x02020000 + (entry << 3));
            *(u32 *)(0x06002840 + offset) = *(u32 *)(0x02020004 + (entry << 3));
        }
        cell = (u32 *)((s32)cell + rowAdvance);
    }
}
