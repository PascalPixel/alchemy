#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_020009f8();
s32 ObjectMotion_ArmCallback();
s32 Func_02000a60();
s32 Object_GetById();
s32 ObjectDispatch_SetSingleChildFlags();
s32 ObjectMotion_SetActionVariant();
s32 Func_02000a48();
extern u8 gPartyState[];
extern u8 MsgNoWay[];
extern u8 MsgILlDoIt[];

s32 Func_0200086c(void)
{
    s32 t0;
    s32 t11;
    s32 t13;
    s32 t15;

    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x209;
    if (FIELD_AT(gPartyState, u16, 480) == (s32)MsgNoWay) {
        t0 = Value1(GameFlag_Test, 0x301);
        if (t0 != 0) {
            Call3(Func_020009f8, 0xe, 0x2580000, 0x1680000);
            Call3(Func_020009f8, 0x10, 0x2780000, 0x15a0000);
            Call3(ObjectMotion_ArmCallback, 0x10, 0x4000, 0);
            Call3(Func_020009f8, 0x11, 0, 0);
        } else {
            Call2(Func_02000a60, 0xe, 0);
        }
        Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 0x11), 0);
        Call2(ObjectMotion_SetActionVariant, 0xb, 2);
    }
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgILlDoIt) {
        Call1(Func_02000a48, 0);
        Call2(ObjectMotion_SetActionVariant, 0xc, 2);
        t11 = Value1(Object_GetById, 0xc);
        FIELD_AT((t11 + 0x59), u8, 0) = (FIELD_AT((t11 + 0x59), u8, 0) | 4);
        Call2(ObjectMotion_SetActionVariant, 0xa, 2);
        t13 = Value1(Object_GetById, 0xa);
        FIELD_AT((t13 + 0x59), u8, 0) = (FIELD_AT((t13 + 0x59), u8, 0) | 4);
        Call2(ObjectMotion_SetActionVariant, 0xb, 2);
        t15 = Value1(Object_GetById, 0xb);
        FIELD_AT((t15 + 0x59), u8, 0) = (4 | FIELD_AT((t15 + 0x59), u8, 0));
    }
    return 0;
}

