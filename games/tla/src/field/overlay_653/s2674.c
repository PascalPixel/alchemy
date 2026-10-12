#include "local.h"

void Func_02002674(void)
{
    ((struct ObjectFlags23 *)Object_GetById(gPartyState.current_owner))->flags |= 32;
    ObjectMotion_SetActionVariant(gPartyState.current_owner, 1);
}

void Func_0200269c(void)
{
    Object_GetById(gPartyState.current_owner)->priority_flags &= ~32;
    Object_GetById(gPartyState.current_owner)->priority_flags |= 1;
}

void Func_020026cc(void)
{
    Func_020051c0();
    Func_02005370(0);
    Motion_CamBounds(0x1480000, -1, 0xae0000, 1);
    Func_020052e8();
    Battle_WaitMode0(20);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset(gPartyState.current_owner, 328, 174);
    Motion_SetVarCbAndRefresh(11, 2);
    Func_020053d8(146);
    Object_SetModeById(11, 2);
    Object_GetById(11)->velocity_y = 0x40000;
    Battle_WaitMode0(10);
    Func_020053d8(146);
    Object_SetModeById(12, 2);
    Object_SetModeById(13, 2);
    Object_GetById(12)->velocity_y = 0x60000;
    Object_GetById(13)->velocity_y = 0x60000;
    ObjectMotion_ResetAndSetPosition(12, 300, 164);
    ObjectMotion_SetPositionAndCommit(13, 356, 164);
    Battle_WaitMode0(40);
    /* FAKEMATCH: the loop note ends the scheduling block before the battle rule is set: without it the native order of the last position call's arguments (r1 shifted before r2 is loaded) is one reordered pair. */
    do {
    } while (0);
    gPartyState.battle_rule_24b = 2;
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov653Room3, 3);
    Func_02005300(9, 4);
    Func_020051c8();
}

void Func_020027b4(void)
{
    Func_020053d0_(76, 0, 2, 2, 73, 14, 1);
}

void Func_020027d4(void)
{
    Func_020053d0_(76, 0, 1, 1, 75, 16, 1);
}

void Func_020027f4(void)
{
    Func_020053d0_(76, 0, 1, 1, 74, 18, 1);
}

void Func_02002814(void)
{
    Func_020053d0_(76, 0, 1, 1, 80, 18, 1);
}

void Func_02002834(void)
{
    Func_020053d0_(76, 0, 1, 1, 87, 15, 1);
}

s32 Func_02002854(void)
{
    s32 scene;

    ((struct EventWork1ac *)Ram_HeapSlots->event_work)->value = 0x204;
    scene = gPartyState.scene;
    if (scene == (s32)&SceneId_Ov653Room1)
        Func_0200290c();
    else if (scene == (s32)&SceneId_Ov653Room2)
        Func_02002a5c();
    else if (scene == (s32)&SceneId_Ov653Room3)
        Func_02002a84();
    else if (scene == (s32)&SceneId_Ov653Room4)
        Func_02002c1c();
    else if (scene == (s32)&SceneId_Ov653Room5)
        Func_02003a14();
    else if (scene == (s32)&SceneId_Ov653Room6)
        Func_02003a5c();
    else if (scene == (s32)&SceneId_Ov653Room7)
        Func_02003a8c();
    else if (scene == (s32)&SceneId_Ov653Room8)
        Func_02003b2c();
    else if (scene == (s32)&SceneId_Ov653Room9)
        Func_02003c8c();
    return 0;
}
