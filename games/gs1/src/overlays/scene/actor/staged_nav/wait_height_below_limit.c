#include "types.h"

/*
 * Staged actor height wait for overlay resource_3b3. The callee name refers
 * to its own call word rather than to a shared runtime address.
 */

void Func_020037f0();

/*
 * Polls for up to sixty ticks until the height at +12 falls to the target
 * at +20 or to limit, then clears +0x28 and parks +0x3c. The height is not
 * mirrored back from +20 afterwards. The owner at 0x02000da8 is 52 bytes
 * and carries no pool.
 */
void Actor_WaitHeightBelowLimit(u8 *obj, s32 limit)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        Func_020037f0(1);
        if (*(s32 *)(obj + 12) <= *(s32 *)(obj + 20)) {
            break;
        }
        if (*(s32 *)(obj + 12) <= limit) {
            break;
        }
        cnt--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
}
