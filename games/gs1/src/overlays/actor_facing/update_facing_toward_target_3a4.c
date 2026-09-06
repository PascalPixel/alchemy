#include "types.h"
#include "facing_object.h"

#define NULL ((void *)0)

s32 Func_02003afa(s32, s32);
#define CalculateFacingAngle Func_02003afa

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object) {
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8) (0xFE & object->facing_flags);
        angle = (u16) CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16) (old + delta);
        }
    }
    return 1;
}
