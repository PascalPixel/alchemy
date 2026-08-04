#include "types.h"

/*
 * Resource 3b3 unindexed helper at 0x02000da8 (52 bytes, 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` at 0x02000da8, epilogue `pop {r5,r6,r7} / pop {r0} /
 * bx r0` at 0x02000dd4-0x02000dd8, immediately followed with no gap by
 * the next owner's push {r5,r6,r7,lr} plus r8 spill at 0x02000ddc,
 * already this overlay's row `0x02000ddc | 2 calls`, so the span is
 * exactly 0x02000da8-0x02000ddc, 52 bytes.
 *
 * A close sibling of 0x02000d78 (same 60-iteration poll-with-timeout
 * shape, same field offsets), but this variant takes an extra `limit`
 * argument and stops as soon as either `obj[+12] <= obj[+20]` or
 * `obj[+12] <= limit`; unlike 0x02000d78 it does not mirror
 * `obj[+20]` back into `obj[+12]` afterward.
 *
 * Raw callee naming.
 */

void Func_020037f0();

void Func_02000da8(u8 *obj, s32 limit)
{
    s32 count = 60;

    for (;;) {
        if (count == 0) {
            break;
        }
        Func_020037f0(1);
        if (*(s32 *)(obj + 12) <= *(s32 *)(obj + 20)) {
            break;
        }
        if (*(s32 *)(obj + 12) <= limit) {
            break;
        }
        count--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
}
