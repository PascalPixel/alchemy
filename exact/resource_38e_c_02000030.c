#include "types.h"
#include "facing_object.h"
#define NULL ((void *)0)

s32 Func_02000aae(s32, s32);
#define CalculateFacingAngle Func_02000aae

#define UpdateFacingTowardTarget Func_02000030
s32 UpdateFacingTowardTarget(struct FacingObject *object) {
    s32 facing_delta;
    u16 old_facing;
    s32 target_facing;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8) (0xFE & object->facing_flags);
        target_facing = (u16) CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old_facing = object->facing;
        facing_delta = (s16)(target_facing - old_facing);
        if (facing_delta != 0) {
            if (facing_delta > 0x1000) {
                facing_delta = 0x1000;
            }
            /* The load at 0x0200006E owns the Thumb-like data word at 0x02000084. */
            if (facing_delta < (s32)0xF842F001) {
                facing_delta = (s32)0xF842F001;
            }
            object->facing = (u16) (old_facing + facing_delta);
        }
    }
    return 1;
}
