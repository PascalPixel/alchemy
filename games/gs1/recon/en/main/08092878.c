#include "types.h"
#include "fixed_math.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void ObjectLink_RotatePairToward(void *source, void *target)
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
        source_angle_target = (u16)ArcTan2(
            FIELD(target, s32 *, 0x10) - FIELD(source, s32 *, 0x10),
            FIELD(target, s32 *, 8) - FIELD(source, s32 *, 8));
        target_angle = source_angle_target + 0x8000;
        counter = 0;
loop:
        source_angle = FIELD(source, u16 *, 6);
        source_angle_delta = (s16)(source_angle_target - source_angle);
        remaining = 2;
        if (source_angle_delta != 0) {
            if (source_angle_delta > 0x1000)
                source_angle_delta = 0x1000;
            if (source_angle_delta < -0x1000)
                source_angle_delta = -0x1000;
            FIELD(source, u16 *, 6) = source_angle + source_angle_delta;
        } else {
            remaining = 1;
        }

        target_angle_current = FIELD(target, u16 *, 6);
        target_angle_delta = (s16)(target_angle - target_angle_current);
        if (target_angle_delta != 0) {
            if (target_angle_delta > 0x1000)
                target_angle_delta = 0x1000;
            if (target_angle_delta < -0x1000)
                target_angle_delta = -0x1000;
            FIELD(target, u16 *, 6) = target_angle_current + target_angle_delta;
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
