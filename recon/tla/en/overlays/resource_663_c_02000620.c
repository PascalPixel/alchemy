#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_02000348();
s32 Func_02003bc8();
s32 Func_02003c88();
s32 GameFlag_SetBit();
s32 Func_02000384();
s32 GameFlag_ClearBit();
s32 Func_02003b80();
s32 Func_02003bd0();
extern u8 gPartyState[];

void Func_02000620(void)
{
    s32 t10;
    s32 t0;
    s32 t1;

    t0 = Value1(Object_GetById, 0xb);
    t1 = Value3(Func_02000348, 3, 0x46, 0x43);
    Func_02003bc8();
    Call1(Func_02003c88, 0);
    if ((FIELD_AT(t0, s32, 8) >> 20) == 0x20) {
        Call1(GameFlag_SetBit, 0x311);
        Func_02000384(4, 0x46, 0x43, (t1 & 0xff));
        Call4(Func_02000384, 2, 0x21, 0x36, (t1 & 0xff));
    } else {
        Call1(GameFlag_ClearBit, 0x311);
        Func_02000384(4, 0x46, 0x43, ((t1 & 0xff) | 0xff00));
        Call4(Func_02000384, 2, 0x21, 0x36, ((t1 & 0xff) | 0xff00));
    }
    t10 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    if ((FIELD_AT(t10, s32, 12) >> 19) == 0x1a) {
        Call6(Func_02003b80, 0x46, 0x43, 1, 1, 0x21, 0x36);
    }
    Func_02003bd0();
}

