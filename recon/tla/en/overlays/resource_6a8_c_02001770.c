#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02004464();
s32 Func_020045ec();
s32 Func_020044d4();
s32 WaitFrames();
s32 ObjectMotion_Launch();
s32 ObjectMotion_SetAngleToward();
s32 Func_0200446c();
extern u8 gPartyState[];

void Func_02001770(void)
{
    s32 t0;
    s32 t1;

    t0 = Value1(GameFlag_Test, 0xa3d);
    if (t0 == 0) {
        t1 = Value1(GameFlag_Test, 0x40);
        if (t1 == 0) {
            Func_02004464();
            Call1(Func_020045ec, 1);
            Call4(Func_020044d4, 0x14, 0x1980000, 0x910000, 0x4000);
            Call1(WaitFrames, 1);
            Call3(ObjectMotion_Launch, 0x14, 4, 0x14);
            Call3(ObjectMotion_SetAngleToward, 0x14, FIELD_AT(gPartyState, s32, 532), 0);
            FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x16a), u16, 0) = 0xc8;
            Func_0200446c();
        }
    }
}

