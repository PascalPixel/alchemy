/*
 * Draft of overlay 6a5 Func_0200045c. 2 instructions differ: the listing
 * sets r0 before r1 for Object_SetModeById(27, 3), the draft r1 first.
 * Tried: a direct call, a typed inline wrapper (u32 id), a wrapper with its
 * parameters reversed, three minutes of permuter: no change, or the next
 * call (27, 0) swaps as well (4).
 */
#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 GameFlag_SetBit();
s32 Func_02005814();
s32 Func_02005984();
s32 Func_020058c4();
s32 Func_020058fc();
s32 Func_020058dc();
s32 Object_SetModeById();
s32 Battle_WaitMode0();
s32 Func_0200581c();
extern u8 MsgYouReLate[];

void Func_0200045c(void)
{
    s32 t0;
    s32 t2;

    t0 = Value1(GameFlag_Test, 0xa23);
    if (t0 != 0) {
        Call1(GameFlag_SetBit, 0x200);
        t2 = Value1(GameFlag_Test, 0xa21);
        if (t2 == 0) {
            Func_02005814();
            Call1(Func_02005984, 0);
            Call1(Func_020058c4, (s32)MsgYouReLate);
            Func_020058fc(0x1c, 0x103, 0x1e);
            Call2(Func_020058dc, 0x1c, 0);
            Call2(Object_SetModeById, 0x1b, 3);
            Call1(Battle_WaitMode0, 0x14);
            Call2(Func_020058dc, 0x1b, 0);
            Func_0200581c();
        }
    }
}

