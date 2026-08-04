#include "types.h"

/* Raise the pending bit on the input record, then refresh the halfword at
 * +30 of its object from the shared workspace entry at +282.
 *
 * STILL-OPEN (8 differing bytes / 36 span): two independent-load pairs are
 * transposed relative to the reference (`ldr r4,[r0,#80]` / `ldr r3,[pc,#28]`
 * swapped, and `strb r3,[r0,#0]` / `lsls r2,r2,#1` swapped). alchemist.ts
 * refused (tiers class,depend-count,priority,unaligned -- no
 * original-order row), and reordering the corresponding C declarations
 * (object before workspace) confirmed by hand that source order does not
 * move these two loads; the scheduler's pick is decided elsewhere.
 */
s32 Func_020002f0(void *arg0)
{
    u8 *record = arg0;
    u8 *workspace = *(u8 **)0x03001e70;
    u8 *object = *(u8 **)(record + 80);
    u8 orred = (u8)(record[89] | 1);

    record[89] = orred;
    *(u16 *)(object + 30) = *(u16 *)(workspace + 282);
    return 1;
}
