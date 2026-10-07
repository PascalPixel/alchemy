#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_02001be4();
s32 Func_02001cfc();
s32 Func_02001d4c();
s32 Func_02001bc4();
s32 Object_GetById();
s32 ObjectMotion_SetSpeedParameters();
s32 Object_SetModeById();
s32 ObjectMotion_CenterOnColumn();
s32 Battle_WaitMode0();
s32 Func_02001cd4();
s32 Event_FinishScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_02001bec();
extern u8 gPartyState[];

void Func_02000098(s32 a0, s32 a1)
{
    s32 t4;

    Func_02001be4();
    Call1(Func_02001cfc, 0);
    Call1(Func_02001d4c, 0x9e);
    Call3(Func_02001bc4, FIELD_AT(a0, s32, 0), FIELD_AT(a0, u16, 4), FIELD_AT(a0, u16, 6));
    t4 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    FIELD_AT((t4 + 0x55), u8, 0) = 2;
    Call3(ObjectMotion_SetSpeedParameters, FIELD_AT(gPartyState, s32, 532), 0x8000, 0x4000);
    Call2(Object_SetModeById, FIELD_AT(gPartyState, s32, 532), 2);
    Call3(ObjectMotion_CenterOnColumn, FIELD_AT(gPartyState, s32, 532), 2, -8);
    Call1(Battle_WaitMode0, 0xa);
    Call1(Func_02001cd4, a1);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02001bec();
}

