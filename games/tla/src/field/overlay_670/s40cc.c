#include "ov670.h"

void Func_02002eb0(void);
void Func_020032e0(void);
void Func_02003610(void);

void Func_020040cc(void)
{
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    Object_SetPartAttribute(Object_GetById(gPartyState.current_owner), 15);
    Ov670_CamMove(-1, -1, -1, 0);
    Func_02005bbc()->motion_flags = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Ov670_TaskAdd(Func_02002ccc, 0x480);
    Ov670_Wait(40);
    Ov670_SePlay(78);
    Ov670_CamSpeed(0xcccc, 0x1999);
    Ov670_CamMove(132 << 17, -1, 194 << 17, 1);
    Func_02005bb4();
    Ov670_ObjTurn(17, 0x3000);
    Motion_SetModeAndWaitAnimation(17, 3);
    Func_02005b4c((s32)MsgLetUsNow);
    Ov670_MessagePut(0x4000 + 17, 0);
    Ov670_SePlay(26);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(27), 0);
    Object_GetById(27)->motion_flags = 0;
    Ov670_ObjSetPos(Object_GetById(27), 0x1090000, 0x180000, 172 << 17);
    Ov670_CamSpeed(0x6666, 0xccc);
    Ov670_CamMove(132 << 17, -1, 170 << 17, 1);
    Ov670_ObjSpeed(17, 0x3333, 0x1999);
    Ov670_ObjWalk(17, 264, 374);
    WaitFrames(1);
    Ov670_TaskAdd(Func_02002eb0, 0x480);
    Ov670_Wait(20);
    Scheduler_RemoveCallbackFar(Func_02002ccc);
    Func_020032e0();
    Ov670_TaskAdd(Func_02002ccc, 0x480);
    Ov670_MessagePut(17, 0);
    Ov670_SePlay(11);
    Func_02003610();
    Ov670_Wait(160);
    Func_02005b5c(17, 0, 80);
    Ov670_SePlay(0x240);
    Func_02003640(1, 1);
    Ov670_Wait(20);
    Func_02005b5c(17, 0, 40);
    Scheduler_RemoveCallbackFar(Func_02002ccc);
    WaitFrames(1);
    ((struct Ov670EventWork *)Ram_HeapSlots->event_work)->screen_effect = 0x202;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02005bc4(10);
}
