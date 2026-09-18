#include "TYPES.H"
#include "FIXED_MATH.H"

struct ObjectPairPosition {
    u8 unknown_00[6];
    s16 angle;
    s32 x;
    s32 y;
    s32 z;
};

s32 ArcTan2(s32, s32);
s32 WaitFrames(s32);

void ObjectLink_RotatePairToward(struct ObjectPairPosition *source,
                                 struct ObjectPairPosition *target)
{
    s32 source_angle_delta;
    s32 target_angle_delta;
    u32 target_angle;
    u32 remaining;
    s32 counter;
    u16 source_angle;
    u16 target_angle_current;
    u32 source_angle_target;

    if (source != 0 && target != 0) {
        source_angle_target = (u16)ArcTan2(target->z - source->z, target->x - source->x);
        target_angle = source_angle_target + 0x8000;
        counter = 0;
loop:
        source_angle = source->angle;
        source_angle_delta = (s16)(source_angle_target - source_angle);
        remaining = 2;
        if (source_angle_delta != 0) {
            if (source_angle_delta > 0x1000)
                source_angle_delta = 0x1000;
            if (source_angle_delta < -0x1000)
                source_angle_delta = -0x1000;
            source->angle = source_angle + source_angle_delta;
        } else {
            remaining = 1;
        }

        target_angle_current = target->angle;
        target_angle_delta = (s16)(target_angle - target_angle_current);
        if (target_angle_delta != 0) {
            if (target_angle_delta > 0x1000)
                target_angle_delta = 0x1000;
            if (target_angle_delta < -0x1000)
                target_angle_delta = -0x1000;
            target->angle = target_angle_current + target_angle_delta;
        } else {
            remaining--;
        }

        if (remaining != 0) {
            WaitFrames(1U);
            counter++;
            if (counter <= 0x3b)
                goto loop;
        }
    }
}
