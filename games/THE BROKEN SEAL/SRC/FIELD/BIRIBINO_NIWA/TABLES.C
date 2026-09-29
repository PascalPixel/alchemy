#include "NIWA.H"

s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        tgt = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

u8 *SceneData_GetScriptTable(void)
{
    return Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}
