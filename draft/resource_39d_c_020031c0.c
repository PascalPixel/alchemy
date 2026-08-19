#include "types.h"
/*
 * resource_39d owner at 0x020031c0, 68 bytes.
 *
 * Collects the slot numbers of every record in the scene list whose position
 * falls inside a band derived from the caller's y: five units deep on one axis
 * and three rows tall on the other. The indices are appended to the caller's
 * buffer through a post-incrementing pointer, which is what the single-register
 * `ldmia`/`stmia` pair is.
 */

struct Rec_39d {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};

void Func_020031c0(s32 *out, s32 y)
{
    struct Rec_39d **list = *(struct Rec_39d ***)0x03001ebc;
    s32 base = 64 - (y >> 20);
    s32 low = base + 8;
    s32 high = base + 11;
    s32 slot;

    list = (struct Rec_39d **)((u8 *)list + 20);

    for (slot = 0; slot <= 65; slot++) {
        struct Rec_39d *rec = *list++;

        if (rec != 0) {
            s32 depth = (rec->f8 >> 20) - 4;
            s32 row = rec->f16 >> 20;

            if ((u32)depth <= 4 && low <= row && row < high) {
                *out++ = slot;
            }
        }
    }
}
