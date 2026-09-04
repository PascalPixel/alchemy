#include "types.h"
#include "facing_object.h"

#define SceneActor_UpdateFacingTowardTarget Func_02000030
#define NULL ((void *)0)

s32 Func_0200186e(s32, s32);
#define CalculateFacingAngle Func_0200186e

s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object) {
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8) (0xFE & object->facing_flags);
        tgt = (u16) CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
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
