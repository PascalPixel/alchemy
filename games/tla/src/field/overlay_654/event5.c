#include "shrine_of_the_sea_god.h"

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_02001340(struct ShrineOfTheSeaGodObject *object, struct ShrineOfTheSeaGodObject *target);
void Func_02001658(struct ShrineOfTheSeaGodObject *object, s32 base);
void Func_020018ac(void);

void Func_020018ac(void)
{
    Func_0200142c(Object_GetById(16));
}

void Func_020018bc(void)
{
    struct ShrineOfTheSeaGodObject *leader = Object_GetById(gPartyState.current_owner);
    struct ShrineOfTheSeaGodObject *pillar = Object_GetById(16);

    Func_020039d8();
    Func_02003b08(0);
    ShrineOfTheSeaGod_ObjWalk(gPartyState.current_owner, 408, 360);
    Func_02001340(leader, pillar);
    ShrineOfTheSeaGod_BlendStart(0, 0, 3, 1, 24, 22);
    ShrineOfTheSeaGod_BlendStart(40, 0, 2, 1, 24, 24);
    WaitFrames(1);
    leader->motion_flags = 3;
    ObjectMotion_SetActionVariant(16, 3);
    pillar->priority_flags |= 2;
    Scheduler_AddOrUpdateCallback(Func_020018ac, 0x480);
    Func_02001658(leader, 2);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 1);
    ShrineOfTheSeaGod_ObjSpeed(gPartyState.current_owner, 0x9999, 0x1999);
    ShrineOfTheSeaGod_ObjCommit(gPartyState.current_owner, 406, 360);
    ShrineOfTheSeaGod_ObjCommit(gPartyState.current_owner, 400, 392);
    Func_02003b48(161);
    Func_02003a30(23, 0, 0);
    Func_02003a30(24, 0, 0);
    Scheduler_RemoveCallbackFar(Func_020018ac);
    WaitFrames(1);
    Func_020039e0();
    GameFlag_SetBit(0x232);
}
