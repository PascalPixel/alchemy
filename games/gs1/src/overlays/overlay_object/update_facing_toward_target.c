#include "types.h"
#include "facing_object.h"
#define NULL ((void *)0)

s32 Func_02001a9e(s32, s32);

#define CalculateFacingAngle Func_02001a9e
s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object) {
    s32 delta;
    u16 old;
    s32 ang;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8) (0xFE & object->facing_flags);
        ang = (u16) CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(ang - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The load at 0x0200006E owns the Thumb-like data word at 0x02000084. */
            if (delta < (s32)0xF842F001) {
                delta = (s32)0xF842F001;
            }
            object->facing = (u16) (old + delta);
        }
    }
    return 1;
}
