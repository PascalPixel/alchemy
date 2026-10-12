#include "ov670.h"

struct Ov670EffectWork {
    u8 unknown_000[0x7c4];
    s16 field_7c4;
};

void Func_020037e4(void)
{
    struct Ov670EffectWork *work = Ram_HeapSlots->effect_work;

    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the work pointer before it builds the field's offset (2 differ without it). */
    if (work->field_7c4 != 0) {
        if (GameFlag_Test(0x220))
            Func_02000624();
        else if (GameFlag_Test(0x221))
            Func_02000668();
        else if (GameFlag_Test(0x222))
            Func_020006ac();
        else if (GameFlag_Test(0x223))
            Func_020006f0();
    }
}

void Func_0200384c(void)
{
    Func_02005c14(136, 1);
    Scheduler_AddOrUpdateCallback(Func_020037e4, 0x480);
    Func_02005c1c(gPartyState.current_owner, -1);
    Func_02005c34();
    Field_DispatchTypeHandler(1);
    Func_02005c24();
    Scheduler_RemoveCallbackFar(Func_020037e4);
    Func_02005c2c();
}

void Func_02003898(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Kibonbo4)
        Scheduler_RemoveCallbackFar(Func_02002c34);
    else if (scene == (s32)&SceneId_Kibonbo5)
        Scheduler_RemoveCallbackFar(Func_02002ccc);
    else if (scene == (s32)&SceneId_Kibonbo6)
        Scheduler_RemoveCallbackFar(Func_02002d64);
}

void Func_020038ec(void)
{
    struct Ov670EventWork *work = Ram_HeapSlots->event_work;

    Func_02005a9c();
    Func_02005c3c(0);
    if (GameFlag_Test(0x202))
        Func_02003898();
    Func_02005c9c(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02005bc4(work->exit);
}

void Func_02003930(void)
{
    struct Ov670EventWork *work = Ram_HeapSlots->event_work;

    work->screen_effect = 0x209;
    Func_020038ec();
}

void Func_02003948(void)
{
    struct Ov670EventWork *work = Ram_HeapSlots->event_work;
    struct Ov670Object *leader;
    s32 x;
    s32 px;

    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the party state's address before it builds the owner's offset (2 differ without it). */
    leader = Object_GetById(gPartyState.current_owner);
    Func_02005a9c();
    Func_02005c3c(0);
    Ov670_CamMove(-1, -1, -1, 0);
    Object_SetModeById(gPartyState.current_owner, 2);
    Ov670_ObjSpeed(gPartyState.current_owner, 0x8000, 0x4000);
    x = leader->x;
    px = x >> 16;
    Func_02005a54(leader, ((8 - px % 16) << 16) + x, leader->y, leader->z);
    Func_02005a5c(leader);
    leader->facing = 0xc000;
    if (GameFlag_Test(0x202))
        Func_02003898();
    ObjectMotion_EnableActionAndSetCallback(gPartyState.current_owner, Data_02005cd0);
    WaitFrames(12);
    Func_02005c9c(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02005bc4(work->exit);
}
