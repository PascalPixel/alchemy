#include "KYUDEN.H"

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8)(0xFE & obj->facing_flags);
        angle = (u16)ArcTan2(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (u16)(old + delta);
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
