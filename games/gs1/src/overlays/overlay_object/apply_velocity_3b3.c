#include "types.h"

union MotionWork {
    struct {
        u32 unk_00[2];
        s32 x, y, z;
        u32 unk_14;
        s32 accum_x, accum_y;
        u32 unk_20[4];
        s32 rate_x, rate_y;
        u32 unk_38[3];
        s32 velocity_x, velocity_y, velocity_z;
        u16 *record;
        u8 unk_54[16];
        u16 angle_step;
    } fields;
};

void OverlayObject_ApplyVelocity(union MotionWork *work)
{
    u16 *record;

    work->fields.x += work->fields.velocity_x;
    work->fields.y += work->fields.velocity_y;
    work->fields.z += work->fields.velocity_z;
    work->fields.accum_x += work->fields.rate_x;
    work->fields.accum_y += work->fields.rate_y;
    record = work->fields.record;
    record[15] += work->fields.angle_step;
}
