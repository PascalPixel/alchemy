#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Math_Cosine();
s32 Math_Sine();
s32 Object_SetPartAttribute();
extern u8 IwramMulQ16[];
extern u8 gFrameCount[];

void Func_020008ac(s32 a0)
{
    s32 t1;
    s32 t3;

    t1 = ((s32 (*)())(s32)IwramMulQ16)(Value1(Math_Cosine, ((FIELD_AT(a0, s32, 48) & 0xff) << 11)), ((FIELD_AT(a0, s32, 12) >> 2) + 0xc0000));
    FIELD_AT(a0, s32, 8) = (FIELD_AT(a0, s32, 68) + (t1 << 1));
    t3 = ((s32 (*)())(s32)IwramMulQ16)(Value1(Math_Sine, ((FIELD_AT(a0, s32, 48) & 0xff) << 11)), ((FIELD_AT(a0, s32, 12) >> 2) + 0xc0000));
    FIELD_AT(a0, s32, 16) = (FIELD_AT(a0, s32, 76) + t3);
    FIELD_AT(a0, s32, 12) = (FIELD_AT(a0, s32, 12) + FIELD_AT(a0, s32, 72));
    FIELD_AT(a0, s32, 48) = (FIELD_AT(a0, s32, 48) + 1);
    Call2(Object_SetPartAttribute, a0, ((((u32)(FIELD_AT(gFrameCount, s32, 0) & 2) >> 1) << 3) + ((u32)(FIELD_AT(gFrameCount, s32, 0) & 2) >> 1)));
}

