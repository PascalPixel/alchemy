#include "ov656.h"

extern const u8 Data_02002f1c[];
extern const u8 Data_02002f44[];
extern const u8 Data_02002f88[];
extern const u8 Data_02002fcc[];

void Func_020015c8(void);
void PartyInventory_Remove(s32 item);

// "How did all these monsters get on board?": the scene that clears the ship and sets its flags.
void Func_02001650(void)
{
    Func_02002c68();
    Func_02002da8(0);
    Func_02002d18((s32)MsgScene656No01);
    Ov656_SePlay(78);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Ov656_ObjSpeed(gPartyState.current_owner, 0xcccc, 0x6666);
    Ov656_ObjWalk(gPartyState.current_owner, 808, 208);
    Ov656_CamSpeed(0xcccc, 0x1999);
    Ov656_CamMove(0x3280000, -1, 0xb00000, 1);
    Func_02002cd8(7, gPartyState.current_owner);
    Func_02002cd8(6, gPartyState.current_owner);
    Func_02002cd8(5, gPartyState.current_owner);
    Func_02002cd8(8, gPartyState.current_owner);
    Ov656_WaitFrames(1);
    Ov656_ObjSpeed(7, 0xcccc, 0x6666);
    Ov656_ObjSpeed(6, 0xcccc, 0x6666);
    Ov656_ObjSpeed(5, 0xcccc, 0x6666);
    Ov656_ObjSpeed(8, 0xcccc, 0x6666);
    ObjectMotion_EnableActionAndSetCallback(6, Data_02002f44);
    ObjectMotion_EnableActionAndSetCallback(5, Data_02002f88);
    ObjectMotion_EnableActionAndSetCallback(8, Data_02002fcc);
    Object_SetActionCallbackAndRefreshById(7, Data_02002f1c);
    Ov656_Wait(20);
    Ov656_MessagePut(7, 0);
    Ov656_ObjTurn(7, 0x4000);
    Ov656_MessagePut(7, 0);
    Object_SetModeById(7, 3);
    Ov656_MessagePut(7, 0);
    Motion_SetModeAndWaitAnimation(gPartyState.current_owner, 3);
    Ov656_ObjWalk(7, 808, 148);
    Motion_SetVarCbAndRefresh(7, 2);
    ObjectMotion_SetActionVariant(7, 1);
    Ov656_SePlay(220);
    Scheduler_AddOrUpdateCallback(Func_020015c8, 0x480);
    Ov656_Wait(60);
    Ov656_SePlay(390);
    Ov656_Wait(20);
    Scheduler_RemoveCallbackFar(Func_020015c8);
    Ov656_SePlay(251);
    Ov656_CamSet(0x2b00000, 0, 0x3a00000, 0x2200000);
    Ov656_CamMove(0x3280000, -1, 0x1c00000, 0);
    Ov656_ObjPlace(gPartyState.current_owner, 0x3280000, 0x1e00000);
    Ov656_ObjPlace(7, 0x3280000, 0x1a40000);
    Ov656_ObjPlace(6, 0x3100000, 0x1d40000);
    Ov656_ObjPlace(5, 0x3400000, 0x1d40000);
    Ov656_ObjPlace(8, 0x3180000, 0x1e80000);
    Func_02002c10();
    Ov656_Wait(20);
    Ov656_SePlay(246);
    Ov656_Wait(80);
    ((struct Ov656EventWork *)Ram_HeapSlots->event_work)->screen_effect = 513;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    PartyInventory_Remove(242);
    GameFlag_SetBit(0x983);
    GameFlag_SetBit(0x930);
    GameFlag_SetBit(0x931);
    GameFlag_SetBit(0x933);
    GameFlag_SetBit(0x934);
    GameFlag_SetBit(0x935);
    GameFlag_SetBit(0x936);
    GameFlag_SetBit(0x937);
    GameFlag_SetBit(0x938);
    GameFlag_SetBit(0x939);
    GameFlag_SetBit(0x93a);
    GameFlag_SetBit(0x93b);
    GameFlag_SetBit(0x93c);
    GameFlag_SetBit(0x93e);
    GameFlag_SetBit(0x932);
    GameFlag_SetBit(0x93f);
    gPartyState.owner_override = 1;
    Func_02002d70(90);
}
