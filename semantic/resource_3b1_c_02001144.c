#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x02001144 (76 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` at 0x02001144, epilogue `pop {r5,r6,r7} / pop {r1} / bx
 * r1` at 0x02001186-0x0200118a. The one-word literal pool at 0x0200118c
 * (0x03001ebc, the workspace pointer, see
 * assets/code/resource_3b1_c_02000b84.c / 02000cc8.c) is included per the
 * usual pool rule, immediately followed by the next owner's push
 * {r5,r6,r7,lr} at 0x02001190, already this overlay's row
 * `0x02001190 | 9 calls`, so the span is 0x02001144-0x02001190, 76 bytes.
 *
 * A box-containment scan over a 58-entry pointer table at
 * `workspace + 0x34` (58 = 65 - 8 + 1 iterations, matching the
 * `bls`-inclusive loop bound `r5: 8..65`): returns the first candidate
 * whose `s16` fields at +10 (x) and +18 (z) fall strictly inside a
 * +-12 box around `(x, z)`, or NULL if none does.
 *
 * Raw callee naming.
 */

extern u8 *Data_03001ebc;

u8 *Func_02001144(s32 x, s32 z)
{
    s32 x_min = x - 12;
    s32 x_max = x + 12;
    s32 z_min = z - 12;
    s32 z_max = z + 12;
    u8 **table = (u8 **)(Data_03001ebc + 0x34);
    s32 i;

    for (i = 0; i < 58; i++) {
        u8 *obj = table[i];
        s16 obj_x = *(s16 *)(obj + 10);
        s16 obj_z = *(s16 *)(obj + 18);

        if (x_min < obj_x && obj_x < x_max && z_min < obj_z && z_max > obj_z) {
            return obj;
        }
    }

    return 0;
}
