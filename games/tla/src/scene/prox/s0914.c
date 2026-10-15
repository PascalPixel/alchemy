#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02004464();
s32 Func_020045ec();
s32 Func_0200463c();
s32 Func_02004404();
s32 GameFlag_SetBit();
s32 Battle_WaitMode0();
s32 Func_020044d4();
s32 Func_02004584();
s32 Func_0200456c();
s32 Motion_CamBounds();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_ArmCallback();
s32 Func_0200454c();
s32 Object_SetModeById();
s32 Func_0200451c();
s32 Func_02004534();
s32 Func_0200455c();
s32 Motion_SetModeAndWaitAnimation();
s32 Func_0200446c();
extern u8 MsgProx47[];

void Func_02000914(void)
{
    s32 t0;
    s32 t8;

    t0 = Value1(GameFlag_Test, 0xa3d);
    if (t0 != 0) {
    } else {
        Func_02004464();
        Call1(Func_020045ec, 0);
        Call1(Func_0200463c, 0x9e);
        Call6(Func_02004404, 0x59, 0, 0x4d, 0x10, 1, 2);
        Call1(GameFlag_SetBit, 0x200);
        Call1(Battle_WaitMode0, 0xa);
        {
            s32 y = 0xc0;
            Call4(Func_020044d4, 8, 0x2080000, 0x1680000, y << 6);
        }
        t8 = Func_02004584();
        FIELD_AT((t8 + 0x55), u8, 0) = t0;
        Call2(Func_0200456c, 0x19999, 0x3333);
        Call4(Motion_CamBounds, 0x2280000, -1, 0x1840000, 1);
        Call1(Battle_WaitMode0, 0xa);
        Call3(ObjectMotion_SetSpeedParameters, 8, 0x13333, 0x9999);
        Call3(ObjectMotion_SetPositionAndReset, 8, 0x20e, 0x180);
        Call3(ObjectMotion_ArmCallback, 9, 0x8000, 0);
        Call3(ObjectMotion_SetPositionAndReset, 8, 0x21c, 0x184);
        Call2(Func_0200454c, 8, 0);
        Call2(Object_SetModeById, 8, 4);
        Call1(Func_0200451c, (s32)MsgProx47);
        Call2(Func_02004534, 0x4008, 0);
        Func_0200455c(9, 0x101, 0);
        Call2(Func_02004534, 9, 0);
        Call2(Object_SetModeById, 8, 3);
        Call2(Func_02004534, 0x4008, 0);
        Func_0200455c(9, 0x100, 0x14);
        Call3(ObjectMotion_ArmCallback, 9, 0xb000, 0);
        Call2(Func_02004534, 9, 0);
        Call3(ObjectMotion_ArmCallback, 8, 0xb000, 0x28);
        Call2(Func_0200454c, 8, 0);
        Call2(Object_SetModeById, 8, 4);
        Call2(Func_02004534, 0x4008, 0);
        Call2(Func_0200454c, 9, 0x8000);
        Call2(Motion_SetModeAndWaitAnimation, 9, 3);
        Call2(Func_02004534, 9, 0);
        Func_0200455c(8, 0x109, 0x14);
        Call2(Func_02004534, 0x4008, 0);
        Call2(Object_SetModeById, 9, 3);
        Call2(Func_02004534, 9, 0);
        Call3(ObjectMotion_ArmCallback, 9, 0xb000, 0);
        Call2(Func_0200454c, 8, 0xb000);
        Call1(GameFlag_SetBit, 0x9c0);
        Func_0200446c();
    }
}

