#include "lemurian_ship.h"

void Func_02000af4(void)
{
    struct LemurianShipObject *leader;

    if (!GameFlag_Test(0x983)) {
        leader = Object_GetById(gPartyState.current_owner);
        Func_02002c68();
        Func_02002da8(0);
        LemurianShip_ObjSpeed(gPartyState.current_owner, 0xcccc, 0x6666);
        LemurianShip_ObjWalk(gPartyState.current_owner, 812, 72);
        Func_02002de0(243);
        LemurianShip_ObjEmoteWait(gPartyState.current_owner, 0x102);
        Func_02002c48(0, 0x20000, 0x10000);
        Battle_WaitMode0(20);
        Func_02002c48(0, 0, 0);
        LemurianShip_MapCopy(68, 68, 50, 64, 3, 3);
        leader->field_22 = 2;
        Battle_WaitMode0(20);
        ObjectMotion_Launch(gPartyState.current_owner, 4, 0);
        LemurianShip_ObjSpeed(gPartyState.current_owner, 0x33333, 0x19999);
        LemurianShip_ObjCommit(gPartyState.current_owner, 812, 88);
        Battle_WaitMode0(20);
        Func_02002de0(232);
        LemurianShip_MapCopy(72, 68, 50, 64, 3, 3);
        Battle_WaitMode0(20);
        leader->field_22 = 1;
    }
}

void Func_02000bf0(s32 id)
{
    Func_02002c68();
    Func_02002da8(0);
    ObjectMotion_SetAngleToward(id, gPartyState.current_owner, 0);
    ObjectMotion_SetVariantCallback(id, 2);
    LemurianShip_ObjEmoteWait(id, 0x102);
    Battle_WaitMode0(20);
    gPartyState.battle_rule_24b = 2;
    Func_02002d78(11, 0);
    GameFlag_SetBit(id + 0x2f8);
}

void Func_02000c50(s32 id)
{
    Func_02002c68();
    Func_02002da8(0);
    ObjectMotion_SetAngleToward(id, gPartyState.current_owner, 0);
    ObjectMotion_SetVariantCallback(id, 2);
    LemurianShip_ObjEmoteWait(id, 0x102);
    Battle_WaitMode0(20);
    gPartyState.battle_rule_24b = 2;
    Func_02002d78(11, 0);
    GameFlag_SetBit(id + 0x2f9);
}

void Func_02000cb0(s32 id)
{
    Func_02002c68();
    Func_02002da8(0);
    ObjectMotion_SetAngleToward(id, gPartyState.current_owner, 0);
    ObjectMotion_SetVariantCallback(id, 2);
    LemurianShip_ObjEmoteWait(id, 0x102);
    Battle_WaitMode0(20);
    gPartyState.battle_rule_24b = 2;
    Func_02002d78(11, 0);
    GameFlag_SetBit(id + 0x2ff);
}

void Func_02000d10(s32 id)
{
    struct LemurianShipObject *object = Object_GetById(id);

    LemurianShip_ObjSpeed(id, 0x33333, 0x19999);
    object->field_48 = 0x8000;
    object->velocity_y = 0x80000;
    ObjectMotion_ResetAndSetPosition(id, 120, 120);
}

void Func_02000d44(void)
{
    Func_02002c68();
    Func_02002da8(0);
    Func_02002de0(0);
    ObjectMotion_SetVariantCallback(9, 2);
    ObjectMotion_SetVariantCallback(10, 2);
    ObjectMotion_SetVariantCallback(11, 2);
    ObjectMotion_SetVariantCallback(12, 2);
    Motion_SetVarCbAndRefresh(13, 2);
    Battle_WaitMode0(20);
    Func_02002de0(147);
    Func_02000d10(9);
    Func_02000d10(10);
    Func_02000d10(11);
    Func_02000d10(12);
    Func_02000d10(13);
    ObjectMotion_CommitCurrentPositionAndActivate(13);
    Func_02002cd0(9, 0, 0);
    Func_02002cd0(10, 0, 0);
    Func_02002cd0(11, 0, 0);
    Func_02002cd0(12, 0, 0);
    Func_02002cd0(13, 0, 0);
    Battle_WaitMode0(20);
    Func_02002de0(52);
    Object_SetActionCallbackAndRefreshById(8, Data_02002e9c);
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the party state address before it builds the field offset (2 differ without it). */
    gPartyState.battle_rule_24b = 2;
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov656Room2, 15);
    Func_02002d78(11, 1);
    GameFlag_SetBit(0x30f);
}
