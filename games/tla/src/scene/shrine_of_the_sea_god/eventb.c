#include "shrine_of_the_sea_god.h"

void Func_02002384(void)
{
    u8 off = 0;

    Func_020039d8();
    Func_02003b08(0);
    Func_02003ad0()->motion_flags = off;
    ShrineOfTheSeaGod_CamMove(480 << 16, 32 << 16, 340 << 16, 1);
    Func_02003ac8();
    ObjectMotion_SetAngleToward(15, gPartyState.current_owner, 0);
    Func_02003aa8(15, 0x100, 40);
    ObjectMotion_SetActionVariant(15, 1);
    Object_GetById(15)->motion_flags = off;
    ShrineOfTheSeaGod_ObjSpeed(15, 0x19999, 0xcccc);
    ShrineOfTheSeaGod_ObjWalk(15, 500, 344);
    ShrineOfTheSeaGod_ObjWalk(15, 580, 344);
    Object_LinkObjectAndSetCallback(15, gPartyState.current_owner);
    ShrineOfTheSeaGod_Wait(20);
    GameFlag_SetBit(0x874);
    Func_020039e0();
}

void Func_02002430(void)
{
    u8 off = 0;

    Func_020039d8();
    Func_02003b08(0);
    ObjectMotion_EnableActionAndResetMotion(15);
    Func_02003ad0()->motion_flags = off;
    ShrineOfTheSeaGod_CamMove(564 << 16, 32 << 16, 344 << 16, 1);
    Func_02003ac8();
    ObjectMotion_SetAngleToward(15, gPartyState.current_owner, 0);
    Func_02003aa8(15, 0x100, 40);
    ObjectMotion_SetActionVariant(15, 1);
    Object_GetById(15)->motion_flags = off;
    ShrineOfTheSeaGod_ObjSpeed(15, 0x19999, 0xcccc);
    ShrineOfTheSeaGod_ObjWalk(15, 616, 376);
    ShrineOfTheSeaGod_ObjWalk(15, 616, 460);
    ShrineOfTheSeaGod_ObjPlace(15, 0, 0);
    ShrineOfTheSeaGod_Wait(20);
    GameFlag_SetBit(0x875);
    Func_020039e0();
}
