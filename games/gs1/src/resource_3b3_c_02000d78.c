#include "types.h"

/*
 * Resource 3b3 unindexed helper at 0x02000d78 (48 bytes, 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` at 0x02000d78, epilogue `pop {r5,r6} / pop {r0} / bx r0`
 * at 0x02000da2-0x02000da6, immediately followed with no gap by the
 * next function's `push {r5,r6,r7,lr}` at 0x02000da8 (not part of this
 * overlay's item-28 population, already indexed elsewhere), so the span
 * is exactly 0x02000d78-0x02000da8, 48 bytes.
 *
 * A poll-with-timeout: up to 60 iterations calling `Func_020037be(1)`
 * (presumably a one-frame wait) until `obj[+12] == obj[+20]`, then
 * resets `obj[+0x28]` to 0, `obj[+0x3c]` to 0x80000000, and mirrors
 * `obj[+20]` back into `obj[+12]`.
 *
 * Raw callee naming.
 */

void Func_020037be();

void Func_02000d78(u8 *obj)
{
    s32 count = 60;

    for (;;) {
        if (count == 0) {
            break;
        }
        Func_020037be(1);
        if (*(u32 *)(obj + 12) == *(u32 *)(obj + 20)) {
            break;
        }
        count--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
    *(u32 *)(obj + 12) = *(u32 *)(obj + 20);
}
