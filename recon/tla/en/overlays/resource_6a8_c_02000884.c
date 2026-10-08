#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02004464();
s32 Func_020045ec();
s32 ObjectMotion_SetAngleToward();
s32 Func_0200451c();
s32 Func_02004534();
s32 Func_0200446c();
extern u8 gPartyState[];
extern u8 MsgScene6A8No115[];
extern u8 MsgScene6A8No119[];

void Func_02000884(void)
{
    s32 t0;

    t0 = Value1(Object_GetById, 0x12);
    Func_02004464();
    Call1(Func_020045ec, 0);
    FIELD_AT((t0 + 0x63), u8, 0) = 1;
    if (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x166), s16, 0) == 0) {
        Call3(ObjectMotion_SetAngleToward, 0x12, FIELD_AT(gPartyState, s32, 532), 0);
        Call1(Func_0200451c, (s32)MsgScene6A8No115);
    } else {
        Call1(Func_0200451c, (s32)MsgScene6A8No119);
    }
    Call2(Func_02004534, 0x12, 0);
    if (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x166), s16, 0) == 0) {
        FIELD_AT(t0, u16, 6) = FIELD_AT(t0, s16, 6);
    }
    FIELD_AT((t0 + 0x63), u8, 0) = 0;
    Func_0200446c();
}

