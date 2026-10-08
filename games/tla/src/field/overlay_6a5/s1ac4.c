#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_020059c4();
s32 Object_GetById();
s32 Func_020059dc();
s32 Func_020059f4();
s32 Battle_WaitMode0();
s32 Func_020059d4();
s32 Func_020059cc();
s32 Object_SetModeById();

void Func_02001ac4(s32 a0, s32 a1)
{
    Func_020059c4();
    Call2(Func_020059dc, Value1(Object_GetById, a0), 2);
    Call1(Func_020059f4, 0xc9);
    Call1(Battle_WaitMode0, 0x3c);
    Call2(Func_020059dc, Value1(Object_GetById, a0), 0);
    Func_020059d4();
    Func_020059cc();
    Call2(Object_SetModeById, a1, 1);
}

