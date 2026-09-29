#include "HAIDIA_BABI.H"

/* The facing controller and the scene hooks the entry veneers export. */

s32 ArcTan2(s32, s32);
s32 FieldScene_PrepareActors(s32);

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
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 *HaidiaBabi_GetEntrances(void)
{
    return gHaidiaBabiEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 HaidiaBabi_SelectExits(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return (s32)gHaidiaBabiExits2;
    }
    return (s32)gHaidiaBabiExits;
}

s32 HaidiaBabi_SelectPlacements(void)
{
    u8 *b = (u8 *)&gGameState;
    s32 *tbl;

    if (*(s16 *)(b + 0x1c2) == 19)
        return (s32)gHaidiaBabiPlacements4;
    if (GameFlag_IsSet(0x87a) != 0)
        tbl = gHaidiaBabiPlacements3;
    else if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
        tbl = gHaidiaBabiPlacements2;
    else
        tbl = gHaidiaBabiPlacements;
    FieldScene_PrepareActors((s32)tbl);
    return (s32)tbl;
}
