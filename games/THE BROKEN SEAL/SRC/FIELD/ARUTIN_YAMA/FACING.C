#include "YAMA.H"

void SceneState_SetValue14Mode23(void)
{
    extern s32 Data_03001e40;

    BattleFx_SetPhaseRequest(0xE, 0x17);
}

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        angle = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
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
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 SceneActor_CopyActor8PositionWithFixedY(struct Frame *dst)
{
    struct Frame *src = Actor_Get(8);

    dst->f08 = src->f08;
    dst->f0c = 0xFFF40000;
    dst->f10 = src->f10;
    return 0;
}

/*
 * Per-frame integrator for one actor record in resource_3a4. Advances the
 * position pair at +8 and +12, advances +24 and +28 by one shared velocity,
 * damps that velocity, and returns 0.
 *
 * The damping subtracts +72 from the value of +40 already held in a register,
 * not from a fresh load; v28 and v2c carry those earlier reads and must stay
 * locals rather than become repeated loads.
 */
s32 OverlayObject_IntegrateAndDamp(u8 *p)
{
    s32 v28;
    s32 v2c;

    *(s32 *)(p + 8) = *(s32 *)(p + 8) + *(s32 *)(p + 36);

    v28 = *(s32 *)(p + 40);
    *(s32 *)(p + 12) = *(s32 *)(p + 12) + v28;

    v2c = *(s32 *)(p + 44);
    *(s32 *)(p + 24) = *(s32 *)(p + 24) + v2c;
    *(s32 *)(p + 28) = *(s32 *)(p + 28) + v2c;

    *(s32 *)(p + 40) = v28 - *(s32 *)(p + 72);

    return 0;
}
