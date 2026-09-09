#include "types.h"

/* Actor placement check for resource_3b1. */

/* Bucket offsets, packed as {s16 hi; s16 lo} per entry. */
extern u32 Data_0200e668[];

u8 *Func_02007772();
s32 Func_020023f0();
s32 Func_0200773c();

/*
 * Offset obj+10 and obj+18 by the bucket's packed hi/lo pair, test the
 * candidate point, and on success pack {x << 16, obj+12, z << 16} into a
 * stack struct for a second check.  Returns 1 only if both checks pass.  The
 * owner includes its one pool word, the bucket table base.  Callees are named
 * by the address their call site computes, not by a runtime address.
 */
s32 SceneActor_CheckBucketOffsetPoint(s32 bucket)
{
    u8 *obj = Func_02007772(0);
    u32 ofs = Data_0200e668[bucket];
    s32 x = *(s16 *)(obj + 10) + ((s32)ofs >> 16);
    s32 z = *(s16 *)(obj + 18) + (s32)(s16)ofs;

    if (Func_020023f0(x, z) != 0) {
        return 0;
    }

    {
        s32 point[3];
        point[0] = x << 16;
        point[1] = *(s32 *)(obj + 12);
        point[2] = z << 16;

        if (Func_0200773c(obj, point) != 0) {
            return 0;
        }
    }

    return 1;
}
