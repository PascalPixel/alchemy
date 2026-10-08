#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Func_0200510c();
s32 Func_020059f4();
s32 Func_0200579c();
s32 Battle_WaitMode0();
extern u8 gPartyState[];

void Func_02005194(s32 a0)
{
    s32 t0;

    t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    Func_0200510c();
    Call1(Func_020059f4, 0xa1);
    Call6(Func_0200579c, FIELD_AT(a0, s16, 6), (FIELD_AT(a0, s16, 8) + 1), 1, 1, ((FIELD_AT(t0, s32, 8) >> 20) | (((0x40 & FIELD_AT(a0, u16, 6)) << 16) >> 16)), ((FIELD_AT(t0, s32, 16) >> 20) | (((0x40 & FIELD_AT(a0, u16, 8)) << 16) >> 16)));
    Call1(Battle_WaitMode0, 0xc);
}

