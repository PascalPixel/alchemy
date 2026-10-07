#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 GameFlag_SetBit();
s32 PartyInventory_Remove();
s32 Func_02001be4();
s32 Func_02001cfc();
s32 Func_02001c74();
s32 Battle_WaitMode0();
s32 ObjectMotion_SetAngleToward();
s32 Func_02001c84();
s32 Object_SetModeById();
s32 ObjectMotion_Launch();
s32 Func_02000398();
s32 ObjectMotion_ArmCallback();
s32 Func_02001bec();
extern u8 MsgThanksMister[];
extern u8 gPartyState[];

void Func_020002f8(void)
{
    Call1(GameFlag_SetBit, 0x8b5);
    Call1(PartyInventory_Remove, 0x1c9);
    Func_02001be4();
    Call1(Func_02001cfc, 0);
    Call1(Func_02001c74, (s32)MsgThanksMister);
    Call1(Battle_WaitMode0, 0xa);
    Call3(ObjectMotion_SetAngleToward, 0x1b, FIELD_AT(gPartyState, s32, 532), 0);
    Call1(Battle_WaitMode0, 0xa);
    Call3(Func_02001c84, 0x1b, 0, 0xa);
    Call3(Func_02001c84, -1, 0, 0xa);
    Call2(Object_SetModeById, 0x1b, 1);
    Call3(ObjectMotion_Launch, 0x1b, 6, 0xf);
    Call3(ObjectMotion_Launch, 0x1b, 6, 0x17);
    Call3(Func_02001c84, 0x1b, 0, 0);
    Call1(Func_02000398, 0x1b);
    Call3(ObjectMotion_ArmCallback, 0x1b, 0x4000, 0);
    Func_02001bec();
}

