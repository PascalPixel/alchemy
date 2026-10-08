#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Math_Cosine();
s32 Math_Sine();
s32 __divsi3();
extern u8 IwramMulQ16[];

void Func_02000840(s32 a0)
{
    s32 t1;
    s32 t4;

    t1 = ((s32 (*)())(s32)IwramMulQ16)(Value1(Math_Cosine, ((FIELD_AT(a0, s32, 48) & 0xff) << 11)), ((FIELD_AT(a0, s32, 12) >> 1) + 0x100000));
    FIELD_AT(a0, s32, 8) = (FIELD_AT(a0, s32, 68) + t1);
    t4 = Value2(__divsi3, ((s32 (*)())(s32)IwramMulQ16)(Value1(Math_Sine, ((FIELD_AT(a0, s32, 48) & 0xff) << 11)), ((FIELD_AT(a0, s32, 12) >> 1) + 0x100000)), 3);
    FIELD_AT(a0, s32, 16) = (FIELD_AT(a0, s32, 76) + t4);
    FIELD_AT(a0, s32, 12) = (FIELD_AT(a0, s32, 12) + FIELD_AT(a0, s32, 72));
    FIELD_AT(a0, s32, 48) = (FIELD_AT(a0, s32, 48) + 1);
}

