#include "TYPES.H"
#include "FACING_OBJECT.H"

s32 ArcTan2(s32, s32);

/* The facing controller: turn toward the facing target by at most 0x1000
   a frame. ☀️ builds it into the Haidia overlay ahead of the scene hooks;
   ⚓️ builds it into the main image after the object actions. */
s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = 0xFE & object->facing_flags;
        tgt = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* In ☀️'s overlay the loader relocates this pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = old + delta;
        }
    }
    return 1;
}
