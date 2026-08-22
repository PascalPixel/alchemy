#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x02001280 (92 bytes incl. pool,
 * 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` + a 12-byte stack buffer at 0x02001280, epilogue
 * `add sp,#12 / pop {r5,r6,r7} / pop {r1} / bx r1` at
 * 0x020012d0-0x020012d6. The one-word literal pool at 0x020012d8
 * (0x0200e668, the same packed `{s16 hi; s16 lo;}` bucket table used by
 * 0x02001190 earlier in this overlay) is included per the usual pool
 * rule, immediately followed by the next owner's push {r5,lr} at
 * 0x020012dc, already this overlay's row `0x020012dc | 9 calls`, so the
 * span is 0x02001280-0x020012dc, 92 bytes.
 *
 * A placement check: offsets `subject[+10]`/`subject[+18]` by the
 * bucket's packed hi/lo pair (the same `BucketTable_0200e668` as
 * 0x02001190), tests the candidate point with `Func_020023f0`, and on
 * success packs `{x<<16, subject[+12], z<<16}` into a stack struct and
 * runs a second check with `Func_0200773c`. Returns 1 only if both
 * checks pass (return 0 on either failure).
 *
 * Raw callee naming.
 */

extern u32 Data_0200e668[];

u8 *Func_02007772();
s32 Func_020023f0();
s32 Func_0200773c();

s32 Func_02001280(s32 bucket)
{
    u8 *subject = Func_02007772(0);
    u32 packed = Data_0200e668[bucket];
    s32 x = *(s16 *)(subject + 10) + ((s32)packed >> 16);
    s32 z = *(s16 *)(subject + 18) + (s32)(s16)packed;

    if (Func_020023f0(x, z) != 0) {
        return 0;
    }

    {
        s32 point[3];
        point[0] = x << 16;
        point[1] = *(s32 *)(subject + 12);
        point[2] = z << 16;

        if (Func_0200773c(subject, point) != 0) {
            return 0;
        }
    }

    return 1;
}
