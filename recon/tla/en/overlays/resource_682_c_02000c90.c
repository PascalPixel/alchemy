#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
void Func_02000c40(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_02002bc4(s32 a);
void Battle_WaitMode0(s32 a);

void Func_02000c90(s32 a0, s32 a1)
{
    s32 t0;
    s32 t1;

    t0 = Object_GetById();
    t1 = Value1(Object_GetById, a1);
    Func_02000c40(FIELD_AT(t0, s32, 8), (FIELD_AT(t0, s32, 12) + 0x1c0000), FIELD_AT(t0, s32, 16), 0x1000, 0xf);
    Call1(Func_02002bc4, 0x97);
    Call1(Battle_WaitMode0, 0xf);
    Func_02000c40(FIELD_AT(t1, s32, 8), (FIELD_AT(t1, s32, 12) + 0x1c0000), FIELD_AT(t1, s32, 16), 0xf000, 0xf);
    Call1(Func_02002bc4, 0x97);
    Call1(Battle_WaitMode0, 0xf);
    Func_02000c40(FIELD_AT(t0, s32, 8), (FIELD_AT(t0, s32, 12) + 0x1c0000), FIELD_AT(t0, s32, 16), 0x1000, 0xf);
    Call1(Func_02002bc4, 0x97);
    Call1(Battle_WaitMode0, 0xf);
    Func_02000c40(FIELD_AT(t1, s32, 8), (FIELD_AT(t1, s32, 12) + 0x1c0000), FIELD_AT(t1, s32, 16), 0xf000, 0xf);
    Call1(Func_02002bc4, 0x97);
    Call1(Battle_WaitMode0, 0xf);
}

