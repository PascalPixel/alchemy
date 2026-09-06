#include "types.h"
#include "facing_object.h"

#define NULL ((void *)0)

s32 Func_02000aae(s32, s32);
#define CalculateFacingAngle Func_02000aae

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
            /* The resource loader decodes the stored literal before use. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16) (old + delta);
        }
    }
    return 1;
}
