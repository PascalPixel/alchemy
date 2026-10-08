#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02002a44();
s32 Func_02002b64();
s32 Func_02002bc4();
s32 Object_GetById();
s32 ObjectMotion_SetSpeedParameters();
s32 Object_SetModeById();
s32 ObjectMotion_CenterOnColumn();
s32 ObjectMotion_OffsetPositionAndResetMotion();
s32 Battle_WaitMode0();
s32 Func_02002b44();
s32 Event_FinishScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_02002a4c();
extern u8 gPartyState[];

void Func_02000080(s32 a0, s32 a1, s32 a2)
{
    s32 t3;

    Func_02002a44();
    Call1(Func_02002b64, 0);
    Call1(Func_02002bc4, 0x9e);
    t3 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    FIELD_AT((t3 + 0x55), u8, 0) = 2;
    Call3(ObjectMotion_SetSpeedParameters, FIELD_AT(gPartyState, s32, 532), 0x8000, 0x4000);
    Call2(Object_SetModeById, FIELD_AT(gPartyState, s32, 532), 2);
    if (a2 == 0) {
        Call3(ObjectMotion_CenterOnColumn, FIELD_AT(gPartyState, s32, 532), 2, -8);
    } else {
        Call3(ObjectMotion_OffsetPositionAndResetMotion, FIELD_AT(gPartyState, s32, 532), 0, -8);
    }
    Call1(Battle_WaitMode0, 0xa);
    Call1(Func_02002b44, a1);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02002a4c();
}

