#include "types.h"

struct ObjectMotionAngleState {
    u8 unknown_000[6];
    u16 current_angle;
    u8 unknown_008[92];
    u16 target_angle;
};

s32 ObjectMotion_StepAngle(struct ObjectMotionAngleState *object)
{
    s32 delta = 0;

    if (object != 0) {
        s32 target_angle = object->target_angle;
        s32 current_angle = object->current_angle;
        delta = (s16)(target_angle - current_angle);
        if (delta != 0) {
            if (delta > 4096) {
                delta = 2048;
            }
            if (delta < -4096) {
                delta = -2048;
            }
            object->current_angle = current_angle + delta;
        }
    }
    return delta;
}
