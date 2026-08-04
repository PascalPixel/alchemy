#include "types.h"

/*
 * Manifest row 08021dfa - two independent leaf functions that set and clear
 * the BG1 priority field.
 *
 * The row is 46 bytes, classified `executable_gap_continuation` /
 * `merge_with_owner`.  The nearest preceding non-continuation row is
 * 08021cb8, whose control-flow walk covers 0x08021cb8..0x08021d70 and stops
 * at its own return; the 138 bytes between that and this row are its literal
 * pool, so this row is not its continuation.  Neither entry here is the
 * target of any branch or pool word in the image - both are called
 * indirectly - and neither has a prologue: each is a handful of instructions
 * ending in `bx lr`.
 *
 * Layout of the row:
 *   0x08021dfa  2 bytes  alignment
 *   0x08021dfc 18 bytes  Func_08021dfc
 *   0x08021e0e  6 bytes  alignment and one pool word (0x0400000a)
 *   0x08021e14 14 bytes  Func_08021e14
 *   0x08021e22  6 bytes  alignment and one pool word (0x0400000a)
 */

#define REG_BG1CNT (*(volatile u16 *)0x0400000a)

/* Put BG1 at the lowest priority. */
void Func_08021dfc(void)
{
    REG_BG1CNT = (REG_BG1CNT & ~3) | 3;
}

/* Put BG1 back at the highest priority. */
void Func_08021e14(void)
{
    REG_BG1CNT = REG_BG1CNT & ~3;
}
