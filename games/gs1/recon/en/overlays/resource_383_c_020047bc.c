#include "types.h"
/*
 * resource_383 owner at 0x020047bc, 64 bytes.
 * Tests whether a record is within eight units of the given point. Each axis
 * falls back to the record's own position when its target field is the
 * unset marker 0x80000000; the comparison is on the squared distance.
 */
struct Rec_383b {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
    u8 pad14[36];
    s32 f56;                    /* +56 */
    u8 pad3c[4];
    s32 f64;                    /* +64 */
};
struct Rec_383b *Func_0200968a();
s32 Func_020047bc(s32 x, s32 y, s32 id)
{
    struct Rec_383b *rec = Func_0200968a(id);
    s32 target_x = rec->f56;
    s32 target_y;
    s32 dx;
    s32 dy;

    if (target_x == (s32)0x80000000) {
        target_x = rec->f8;
    }
    target_y = rec->f64;
    if (target_y == (s32)0x80000000) {
        target_y = rec->f16;
    }

    dx = (target_x - x) >> 16;
    dy = (target_y - y) >> 16;
    return dx * dx + dy * dy <= 64;
}
