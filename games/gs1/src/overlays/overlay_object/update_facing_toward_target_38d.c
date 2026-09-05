#include "types.h"
#include "facing_object.h"

#define NULL ((void *)0)

s32 Func_02002462(s32, s32);
#define CalculateFacingAngle Func_02002462

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj) {
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8) (0xFE & obj->facing_flags);
        angle = (u16) CalculateFacingAngle(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The load at 0x0200006E owns the Thumb-like data word at 0x02000084. */
            if (delta < (s32)0xF842F001) {
                delta = (s32)0xF842F001;
            }
            obj->facing = (u16) (old + delta);
        }
    }
    return 1;
}
