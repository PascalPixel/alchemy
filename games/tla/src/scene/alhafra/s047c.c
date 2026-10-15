#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_SetBit();
s32 Func_02001be4();
s32 Func_02001cfc();
s32 Func_02001c74();
s32 Motion_CamBounds();
void Func_02001ccc(void);
s32 Battle_WaitMode0();
s32 Motion_SetVarCbAndRefresh();
s32 Func_02001c84();
s32 ObjectMotion_ArmCallback();
s32 Object_SetModeById();
s32 Func_02001d4c();
s32 Func_02001cec();
s32 ObjectMotion_Launch();
s32 Motion_SetModeAndWaitAnimation();
s32 Func_02001ca4();
s32 Object_AttachWorkTargetToObject();
s32 Func_02001bec();
extern u8 MsgAlhafra74[];
extern u8 gPartyState[];

void Func_0200047c(void)
{
    Call1(GameFlag_SetBit, 0x8b7);
    Func_02001be4();
    Call1(Func_02001cfc, 0);
    Call1(Func_02001c74, (s32)MsgAlhafra74);
    Call4(Motion_CamBounds, 0x2300000, -1, 0x1f00000, 1);
    Func_02001ccc();
    Call1(Battle_WaitMode0, 0xa);
    Call2(Motion_SetVarCbAndRefresh, 0x1d, 2);
    Call1(Battle_WaitMode0, 0xa);
    Call3(Func_02001c84, 0x1d, 0, 0xa);
    Call3(ObjectMotion_ArmCallback, 0x1c, 0, 0);
    Call3(ObjectMotion_ArmCallback, 0x1f, 0x6000, 0);
    Call3(ObjectMotion_ArmCallback, 0x1e, 0x6000, 0);
    Call2(Object_SetModeById, 0x1d, 5);
    Call1(Func_02001d4c, 0x59);
    FIELD_AT(gPartyState, s32, 532) = 0x1d;
    Call2(Func_02001cec, 0, 3);
    FIELD_AT(gPartyState, s32, 532) = 4;
    Call1(Battle_WaitMode0, 0x32);
    Call3(ObjectMotion_Launch, 0x1c, 6, 0xf);
    Call3(ObjectMotion_Launch, 0x1c, 6, 0x17);
    Call3(Func_02001c84, 0x1c, 0, 0xa);
    Call3(ObjectMotion_ArmCallback, 0x1d, 0x8000, 0);
    Call1(Battle_WaitMode0, 0x14);
    Call2(Motion_SetModeAndWaitAnimation, 0x1d, 3);
    Call1(Battle_WaitMode0, 0x14);
    Func_02001ca4(0x1e, 0x108, 0x1e);
    Call3(Func_02001c84, 0x1e, 0, 0xa);
    Func_02001ca4(0x1d, 0x102, 0x28);
    Call3(ObjectMotion_ArmCallback, 0x1d, 0x4000, 0);
    Call1(Battle_WaitMode0, 0x14);
    Call2(Motion_SetVarCbAndRefresh, 0x1d, 2);
    Call1(Battle_WaitMode0, 0xa);
    Call3(Func_02001c84, 0x1d, 0, 0xa);
    Call2(Motion_SetVarCbAndRefresh, 0x1e, 2);
    Call1(Battle_WaitMode0, 0xa);
    Call3(Func_02001c84, 0x1e, 0, 0xa);
    Call3(ObjectMotion_ArmCallback, 0x1d, 0, 0);
    Call1(Battle_WaitMode0, 0x14);
    Call2(Motion_SetModeAndWaitAnimation, 0x1d, 3);
    Call1(Battle_WaitMode0, 0x14);
    Call3(ObjectMotion_Launch, 0x1f, 6, 0xf);
    Call3(ObjectMotion_Launch, 0x1f, 6, 0x17);
    Call3(Func_02001c84, 0x1f, 0, 0xa);
    Call3(ObjectMotion_ArmCallback, 0x1e, 0, 0);
    Call2(Motion_SetModeAndWaitAnimation, 0x1e, 3);
    Call1(Battle_WaitMode0, 0x14);
    Call3(ObjectMotion_ArmCallback, 0x1f, 0x4000, 0);
    Call2(Object_SetModeById, 0x1f, 6);
    Call3(ObjectMotion_ArmCallback, 0x1e, 0xc000, 0);
    Call2(Object_SetModeById, 0x1e, 6);
    Call3(ObjectMotion_ArmCallback, 0x1d, 0x4000, 0);
    Call3(ObjectMotion_ArmCallback, 0x1c, 0xc000, 0);
    Call2(Object_AttachWorkTargetToObject, FIELD_AT(gPartyState, s32, 532), 1);
    Func_02001ccc();
    Func_02001bec();
}

