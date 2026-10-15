#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32       GameFlag_SetBit();
s32       Func_02002a44();
s32       Func_02002b64();
s32       Func_02002adc();
s32       ObjectMotion_ArmCallback();
s32       ObjectMotion_Launch();
s32       Func_02002aec();
s32       Motion_CamBounds();
s32       Func_02002b3c();
s32       Battle_WaitMode0();
s32       ObjectMotion_SetVariantCallback();
s32       Func_02002b24();
s32       Func_02002b04();
s32       Func_02002b1c();
s32       ObjectMotion_SetSpeedParameters();
s32       Object_GetById();
s32       ObjectMotion_OffsetPositionAndReset();
s32       ObjectMotion_CommitPositionAndActivate();
s32       Object_LinkObjectAndSetCallback();
s32       Func_02002a9c();
void      Func_02002a4c(void);
extern u8 MsgChampa49[];

void Func_02000188(void)
{
    s32 t24;
    s32 t26;
    s32 t29;

    Call1(GameFlag_SetBit, 0x973);
    Func_02002a44();
    Call1(Func_02002b64, 0);
    Call1(Func_02002adc, (s32)MsgChampa49);
    Call3(ObjectMotion_ArmCallback, 0x20, 0x3000, 0);
    Call3(ObjectMotion_Launch, 0x20, 6, 0x14);
    Call3(Func_02002aec, 0x20, 0, 5);
    Call4(Motion_CamBounds, 0xb00000, -1, 0x1000000, 1);
    Func_02002b3c();
    Call1(Battle_WaitMode0, 0xa);
    Call2(ObjectMotion_SetVariantCallback, 0x20, 3);
    Call2(Func_02002b24, 0x20, 0x102);
    Call1(Battle_WaitMode0, 0x28);
    Call2(Func_02002b04, 0x21, 0);
    Call1(Battle_WaitMode0, 0xa);
    Func_02002b1c(0x21, 0x101, 0x23);
    Call2(Func_02002b04, 0x21, 0x3000);
    Call1(Battle_WaitMode0, 0xa);
    Func_02002b1c(0x21, 0x100, 0x1e);
    Call3(Func_02002aec, 0x21, 0, 5);
    Call3(ObjectMotion_Launch, 0x20, 6, 0);
    Call3(ObjectMotion_Launch, 0x21, 6, 0);
    Call3(ObjectMotion_SetSpeedParameters, 0x20, 0x1cccc, 0xe666);
    Call3(ObjectMotion_SetSpeedParameters, 0x21, 0x1cccc, 0xe666);
    t24 = Value1(Object_GetById, 0x20);
    FIELD_AT((t24 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t24 + 0x5a), u8, 0));
    Call3(ObjectMotion_OffsetPositionAndReset, 0x20, 0, -16);
    t26 = Value1(Object_GetById, 0x21);
    FIELD_AT((t26 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t26 + 0x5a), u8, 0));
    Call3(ObjectMotion_CommitPositionAndActivate, 0x21, 0, -16);
    Call1(Battle_WaitMode0, 1);
    t29 = Value1(Object_GetById, 0x21);
    FIELD_AT((t29 + 0x5a), u8, 0) = (1 | FIELD_AT((t29 + 0x5a), u8, 0));
    Call1(Battle_WaitMode0, 0xf);
    Func_02002b1c(0x20, 0x109, 0x1e);
    Call3(Func_02002aec, 0x20, 0, 5);
    Call3(ObjectMotion_ArmCallback, 0x20, 0xb000, 0);
    Call2(Func_02002b04, 0x21, 0xd000);
    Call1(Battle_WaitMode0, 5);
    Call3(Func_02002aec, 0x8021, 0, 5);
    Call3(Func_02002aec, 0x22, 0, 5);
    Call2(Object_LinkObjectAndSetCallback, 0x20, 0x22);
    Call2(Object_LinkObjectAndSetCallback, 0x21, 0x22);
    Call3(ObjectMotion_SetSpeedParameters, 0x22, 0x13333, 0x9999);
    Call3(ObjectMotion_CommitPositionAndActivate, 0x22, 0, 0x20);
    Call2(Func_02002b04, 0x22, 0x3000);
    Call4(Motion_CamBounds, 0xd00000, -1, 0x1280000, 1);
    Func_02002b3c();
    Call1(Battle_WaitMode0, 0xf);
    Call3(ObjectMotion_Launch, 0x22, 6, 0xf);
    Call3(ObjectMotion_Launch, 0x22, 6, 0x17);
    Call3(Func_02002aec, 0x22, 0, 5);
    Call3(ObjectMotion_Launch, 0x22, 6, 0x14);
    Call3(ObjectMotion_SetSpeedParameters, 0x22, 0x20000, 0x10000);
    Call3(ObjectMotion_CommitPositionAndActivate, 0x22, 0, -48);
    Call3(ObjectMotion_ArmCallback, 0x20, 0xb000, 0);
    Call3(ObjectMotion_ArmCallback, 0x21, 0xd000, 0);
    Call3(Func_02002a9c, 0x22, 0, 0);
    Call3(ObjectMotion_SetSpeedParameters, 0x20, 0x20000, 0x10000);
    Call3(ObjectMotion_SetSpeedParameters, 0x21, 0x20000, 0x10000);
    Call3(ObjectMotion_CommitPositionAndActivate, 0x20, -16, -16);
    Call3(ObjectMotion_OffsetPositionAndReset, 0x21, 0x10, -16);
    Call3(ObjectMotion_CommitPositionAndActivate, 0x20, 0, -16);
    Call3(Func_02002a9c, 0x20, 0, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 0x21, 0, -16);
    Call3(Func_02002a9c, 0x21, 0, 0);
    Func_02002a4c();
}
