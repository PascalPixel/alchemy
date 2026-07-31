#include "types.h"

/*
 * Manifest row 0801c9be - two empty callbacks and one clearing routine.
 *
 * The row is 94 bytes, classified `executable_gap_continuation` /
 * `merge_with_owner`.  The nearest preceding non-continuation row is
 * 0801c8a0, whose walk covers 0x0801c8a0..0x0801c912 and closes on its own
 * return, and a gap separates the two rows, so this is not its continuation.
 * None of the three entries is the target of a branch or a stored pointer in
 * the image; all three are leaves without a prologue, ending in `bx lr`.
 *
 * Layout of the row:
 *   0x0801c9be  2 bytes  alignment
 *   0x0801c9c0  2 bytes  Func_0801c9c0 - does nothing
 *   0x0801c9c2  2 bytes  alignment
 *   0x0801c9c4  2 bytes  Func_0801c9c4 - does nothing
 *   0x0801c9c6  2 bytes  alignment
 *   0x0801c9c8 78 bytes  Func_0801c9c8
 *   0x0801ca16  6 bytes  alignment and one pool word (0x0000057c)
 *
 * Two do-nothing entries that are distinct addresses are kept distinct here:
 * they are published separately and merging them would change the call
 * multiset.
 *
 * Func_0801c9c8 clears one halfword at offset +10 of each of seven records
 * laid out at a 52-byte stride from +0x400 (the seventh record is reached
 * with a 60-byte step, not 52), then clears the five consecutive halfwords
 * that follow the last record.
 */

void Func_0801c9c0(void)
{
}

void Func_0801c9c4(void)
{
}

void Func_0801c9c8(u8 *state)
{
    s32 offset;
    s32 index;

    offset = 0x400;
    for (index = 0; index < 6; index++) {
        *(u16 *)(state + offset + 10) = 0;
        offset += 52;
    }
    *(u16 *)(state + offset + 10) = 0;
    offset += 60;

    *(u16 *)(state + offset) = 0;
    *(u16 *)(state + offset + 2) = 0;
    *(u16 *)(state + offset + 4) = 0;
    *(u16 *)(state + offset + 6) = 0;
    *(u16 *)(state + offset + 8) = 0;
}
