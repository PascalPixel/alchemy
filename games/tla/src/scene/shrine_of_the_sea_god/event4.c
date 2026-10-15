#include "shrine_of_the_sea_god.h"

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_02001340(struct ShrineOfTheSeaGodObject *object, struct ShrineOfTheSeaGodObject *target);
void Func_02001658(struct ShrineOfTheSeaGodObject *object, s32 base);
void Func_020016c8(void);

void Func_02001658(struct ShrineOfTheSeaGodObject *object, s32 base)
{
    do {
        WaitFrames(1);
    } while (object->speed != 0);
    Func_02003b48(240);
    ObjectMotion_SetActionVariant(base + 21, 3);
    ObjectMotion_SetActionVariant(base + 22, 3);
    Object_SetPositionAndResetMotion(Object_GetById(base + 21), object->x, 0, object->z);
    Object_SetPositionAndResetMotion(Object_GetById(base + 22), object->x, 0, object->z + -0x100000);
    Object_SetModeById(base + 21, 1);
    Object_SetModeById(base + 22, 1);
    WaitFrames(20);
}

void Func_020016c8(void)
{
    Func_0200142c(Object_GetById(15));
}

void Func_020016d8(void)
{
    struct ShrineOfTheSeaGodObject *leader = Object_GetById(gPartyState.current_owner);
    struct ShrineOfTheSeaGodObject *pillar = Object_GetById(15);

    Func_020039d8();
    Func_02003b08(0);
    ShrineOfTheSeaGod_ObjWalk(gPartyState.current_owner, 328, 216);
    Func_02001340(leader, pillar);
    ShrineOfTheSeaGod_BlendStart(0, 0, 3, 1, 19, 13);
    ShrineOfTheSeaGod_BlendStart(40, 0, 2, 1, 19, 33);
    WaitFrames(1);
    leader->motion_flags = 3;
    ObjectMotion_SetActionVariant(15, 3);
    pillar->priority_flags |= 2;
    Scheduler_AddOrUpdateCallback(Func_020016c8, 0x480);
    Func_02001658(leader, 0);
    ObjectMotion_SetActionVariant(9, 1);
    Object_GetById(9)->priority_flags &= ~2;
    ObjectMotion_SetActionVariant(11, 1);
    Object_GetById(11)->priority_flags &= ~2;
    leader->motion_flags = 0;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 1);
    ShrineOfTheSeaGod_ObjSpeed(gPartyState.current_owner, 0x9999, 0x1999);
    ShrineOfTheSeaGod_ObjCommit(gPartyState.current_owner, 328, 224);
    ShrineOfTheSeaGod_ObjCommit(gPartyState.current_owner, 321, 256);
    Func_02003a30(21, 0, 0);
    Func_02003a30(22, 0, 0);
    ShrineOfTheSeaGod_ObjCommit(gPartyState.current_owner, 321, 320);
    ShrineOfTheSeaGod_ObjCommit(gPartyState.current_owner, 321, 400);
    ShrineOfTheSeaGod_ObjCommit(gPartyState.current_owner, 321, 480);
    ShrineOfTheSeaGod_ObjCommit(gPartyState.current_owner, 321, 536);
    Func_02003b48(161);
    ObjectMotion_SetActionVariant(9, 2);
    Object_GetById(9)->priority_flags |= 2;
    ObjectMotion_SetActionVariant(11, 2);
    Object_GetById(11)->priority_flags |= 2;
    leader->motion_flags = 3;
    Scheduler_RemoveCallbackFar(Func_020016c8);
    WaitFrames(1);
    Func_020039e0();
    GameFlag_SetBit(0x231);
}
