#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02001c74();
s32 Func_02001c84();
s32 ObjectMotion_ArmCallback();
s32 Battle_WaitMode0();
s32 Func_02001ca4();
s32 ObjectMotion_SetVariantCallback();
s32 Func_02001cac();
extern u8 MsgScene665No50[];

void Func_02000288(void)
{
    Call1(Func_02001c74, (s32)MsgScene665No50);
    Call3(Func_02001c84, 0x18, 0, 0xa);
    Call3(ObjectMotion_ArmCallback, 0x13, -256, 0);
    Call1(Battle_WaitMode0, 0xa);
    Func_02001ca4(0x13, 0x107, 0x1e);
    Call3(Func_02001c84, 0x13, 0, 0xa);
    Call2(ObjectMotion_SetVariantCallback, 0x18, 3);
    Call2(Func_02001cac, 0x18, 0x102);
    Call1(Battle_WaitMode0, 0x28);
    Call3(ObjectMotion_ArmCallback, 0x13, 0x8000, 0);
    Call3(Func_02001c84, 0x18, 0, 0);
}

