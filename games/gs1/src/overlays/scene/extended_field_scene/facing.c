#include "types.h"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_02002656
#define Object_UpdateFacingTowardTarget Func_02000030

#include "facing_object.h"

s32 Func_02002656(s32, s32);

s32 Object_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 facing_delta;
    u16 old_facing;
    s32 target_facing;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        target_facing = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old_facing = object->facing;
        facing_delta = (s16)(target_facing - old_facing);
        if (facing_delta != 0) {
            if (facing_delta > 0x1000) {
                facing_delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (facing_delta < -0x1000) {
                facing_delta = -0x1000;
            }
            object->facing = (u16)(old_facing + facing_delta);
        }
    }
    return 1;
}
