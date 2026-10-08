#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Math_Cosine();
s32 Math_Sine();

void Func_02000090(s32 a0)
{
    s32 t0;
    s32 t1;
    s32 t2;

    t0 = Value1(Object_GetById, 0x1f);
    t1 = Value1(Math_Cosine, FIELD_AT((a0 + 0x64), u16, 0));
    FIELD_AT(a0, s32, 8) = (FIELD_AT(t0, s32, 8) + ((FIELD_AT(a0, s32, 48) + 3) * t1));
    t2 = Value1(Math_Sine, FIELD_AT((a0 + 0x64), u16, 0));
    FIELD_AT(a0, s32, 16) = (FIELD_AT(t0, s32, 16) + (t2 << 1));
    FIELD_AT(a0, s32, 56) = FIELD_AT(a0, s32, 8);
    FIELD_AT(a0, s32, 64) = (FIELD_AT(t0, s32, 16) + (t2 << 1));
    FIELD_AT((a0 + 0x64), s16, 0) = (FIELD_AT((a0 + 0x64), s16, 0) + -2048);
}

