#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32       Func_02003398();
s32       Func_02003488();
s32       ObjectMotion_SetPositionAndReset();
s32       Battle_WaitMode0();
s32       Func_02003328();
s32       Func_020033e0();
s32       Func_020034b8();
s32       GameFlag_SetBit();
s32       Func_020033a0();
extern u8 gPartyState[];

void Func_02001660(void)
{
    Func_02003398();
    Call1(Func_02003488, 0);
    Call3(ObjectMotion_SetPositionAndReset, FIELD_AT(gPartyState, s32, 532), 0x218, 0x168);
    Call1(Battle_WaitMode0, 8);
    Call6(Func_02003328, 0x32, 0x16, 0x21, 0x16, 1, 1);
    Call3(Func_020033e0, 0xe, 0, 0);
    Call1(Func_020034b8, 0x248);
    Call1(Battle_WaitMode0, 0x1e);
    Call1(Func_020034b8, 0xbc);
    Call6(Func_02003328, 0x32, 0x14, 0x1f, 0x14, 1, 2);
    Call1(Battle_WaitMode0, 5);
    Call6(Func_02003328, 0x33, 0x14, 0x1f, 0x14, 1, 2);
    Call1(Battle_WaitMode0, 5);
    Call1(GameFlag_SetBit, 0x200);
    Func_020033a0();
}
