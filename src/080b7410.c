#include "types.h"

/*
 * Func_080b7410 - read one signed (x, y) pair out of the fixed step table at
 * 0x080c2a62.
 *
 * Manifest row 080b7410 is 20 bytes, classified `executable_gap_continuation`
 * / `merge_with_owner`.  The walk from the nearest preceding
 * non-continuation row, 080b6f44, ends at 0x080b7400 on that function's own
 * return, so this row is not its tail; the row has no prologue, takes three
 * arguments and ends in `bx lr`, which makes it an ordinary leaf rather than a
 * continuation.
 *
 * Its caller is UNKNOWN, not merely indirect.  `tools/main_xref.ts 080b7410`
 * finds no reference of any kind anywhere in the image: no call, no branch, no
 * pool word holding the address, and none holding address + 1.  It is reached
 * by a computed address, from data this scan does not cover, or not at all.
 * The reconstruction below is faithful to the bytes either way; only the
 * question of who runs them is open.
 *
 * Executable range 0x080b7410..0x080b7420, followed by the single pool word
 * holding the table base.
 *
 * The table is signed bytes in (x, y) pairs, so the index is doubled once and
 * the second component read at index+1.
 */

extern const s8 Data_080c2a62[];

void Func_080b7410(s32 index, s32 *x, s32 *y)
{
    index *= 2;
    *x = Data_080c2a62[index];
    *y = Data_080c2a62[index + 1];
}
