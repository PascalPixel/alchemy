#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_SetBit();
s32 Func_02000a94();
s32 Func_02000b8c();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_ArmCallback();
s32 Func_02000b24();
s32 Battle_WaitMode0();
s32 Func_02000b34();
s32 Motion_SetModeAndWaitAnimation();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_CommitPositionAndActivate();
s32 Motion_SetVarCbAndRefresh();
s32 Func_02000b5c();
s32 ObjectMotion_SetVariantCallback();
s32 Func_02000b64();
s32 Object_SetModeById();
void Func_02000a9c(void);
extern u8 gPartyState[];
extern u8 MsgScene666No07[];

void Func_020002bc(void)
{
    Call1(GameFlag_SetBit, 0x8b1);
    Func_02000a94();
    Call1(Func_02000b8c, 0);
    Call3(ObjectMotion_SetPositionAndReset, FIELD_AT(gPartyState, s32, 532), 0x98, 0x148);
    Call3(ObjectMotion_ArmCallback, FIELD_AT(gPartyState, s32, 532), 0xc000, 0);
    Call1(Func_02000b24, (s32)MsgScene666No07);
    Call3(ObjectMotion_ArmCallback, 0xe, 0, 0);
    Call1(Battle_WaitMode0, 0x14);
    Call3(Func_02000b34, 0xe, 0, 0xa);
    Call3(ObjectMotion_ArmCallback, 0xf, 0x8000, 0);
    Call1(Battle_WaitMode0, 0x14);
    Call2(Motion_SetModeAndWaitAnimation, 0xf, 3);
    Call1(Battle_WaitMode0, 0xa);
    Call3(Func_02000b34, 0xf, 0, 0xa);
    Call3(ObjectMotion_SetSpeedParameters, 0xe, 0x8000, 0x4000);
    Call3(ObjectMotion_CommitPositionAndActivate, 0xe, -32, 0);
    Call1(Battle_WaitMode0, 0xa);
    Call3(Func_02000b34, 0xe, 0, 0xa);
    Call2(Motion_SetVarCbAndRefresh, 0xd, 2);
    Call1(Battle_WaitMode0, 0xa);
    Call3(ObjectMotion_ArmCallback, 0xd, 0, 0);
    Call1(Battle_WaitMode0, 0x14);
    Func_02000b5c(0xd, 0x107, 0x1e);
    Call3(Func_02000b34, 0xd, 0, 0xa);
    Call2(ObjectMotion_SetVariantCallback, 0xe, 3);
    Call2(Func_02000b64, 0xe, 0x102);
    Call2(ObjectMotion_SetVariantCallback, 0xf, 3);
    Call2(Func_02000b64, 0xf, 0x102);
    Call1(Battle_WaitMode0, 0x32);
    Call2(Motion_SetVarCbAndRefresh, 0xd, 2);
    Call1(Battle_WaitMode0, 0x14);
    Call3(ObjectMotion_ArmCallback, 0xd, 0xb000, 0);
    Call1(Battle_WaitMode0, 0xa);
    Call3(Func_02000b34, 0xd, 0, 0xa);
    Call1(Battle_WaitMode0, 0xa);
    Call3(ObjectMotion_SetPositionAndReset, 0xe, 0x98, 0x138);
    Call1(Battle_WaitMode0, 0xa);
    Call2(Object_SetModeById, 0xf, 4);
    Call2(Motion_SetModeAndWaitAnimation, 0xe, 4);
    Call1(Battle_WaitMode0, 0xa);
    Call3(ObjectMotion_ArmCallback, 0xe, 0x3000, 0);
    Call3(ObjectMotion_ArmCallback, 0xf, 0x4000, 0);
    Func_02000a9c();
}

