#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x02004fa8 (92 bytes incl. pool,
 * 27 calls -- the highest call count left in the item-28 queue after
 * the top four rows were adopted/drafted at the start of this reconstruction).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` at 0x02004fa8, epilogue `pop {r5,r6,r7} / pop {r1} /
 * bx r1` at 0x02004ff2-0x02004ff6. The trailing pool at
 * 0x02004ff8-0x02005000 (0x0000092c, 0x00000935, 0x00000917 -- plain
 * numeric arguments, the same constants reused as arguments in
 * 0x02005004 immediately after) is included per the usual pool rule,
 * immediately followed by the next owner's push {lr} at 0x02005004,
 * already this overlay's row `0x02005004` (drafted earlier this
 * session), so the span is 0x02004fa8-0x02005004, 92 bytes.
 *
 * `base` is picked by a 4-way switch on `kind` (default 0 for anything
 * outside 0..3); `result_base` is 8 or 18 depending on whether `flag`
 * is zero. Then a 9-step scan (`i` 0..8 inclusive) calls
 * `Func_0200b480(base + i)` and returns `result_base + i` on the first
 * hit, or 0 if none hit.
 *
 * Raw callee naming.
 */

s32 Func_080770c0();

s32 Func_02004fa8(s32 kind, s32 flag)
{
    s32 base;
    s32 result_base = (flag == 0) ? 8 : 18;
    s32 i;

    switch (kind) {
    case 0:
        base = 0x92c;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    default:
        base = 0;
        break;
    }

    for (i = 0; i <= 8; i++) {
        if (Func_080770c0(base + i) != 0) {
            return result_base + i;
        }
    }

    return 0;
}
