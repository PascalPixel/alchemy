#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Math_Cosine();
s32 Math_Sine();
extern u8 IwramMulQ16[];
extern u8 gFrameCount[];

s32 Func_0200142c(s32 a0)
{
    s32 t1;
    s32 t3;
    s32 t4;

    FIELD_AT((a0 + 0x64), u16, 0) = (FIELD_AT((a0 + 0x64), u16, 0) + FIELD_AT((0x66 + a0), u16, 0));
    t1 = ((s32 (*)())(s32)IwramMulQ16)(Value1(Math_Cosine, (((FIELD_AT((a0 + 0x64), u16, 0) + FIELD_AT((0x66 + a0), u16, 0)) << 16) >> 16)), FIELD_AT(a0, s32, 76));
    FIELD_AT(a0, s32, 8) = t1;
    t3 = ((s32 (*)())(s32)IwramMulQ16)(Value1(Math_Sine, FIELD_AT((a0 + 0x64), u16, 0)), FIELD_AT(a0, s32, 76));
    FIELD_AT(a0, s32, 8) = (FIELD_AT(a0, s32, 8) + FIELD_AT(a0, s32, 68));
    FIELD_AT(a0, s32, 16) = ((t3 >> 1) + FIELD_AT(a0, s32, 72));
    if (FIELD_AT(((a0 + 0x64) - 1), u8, 0) != 0x8d) {
        if ((FIELD_AT(gFrameCount, s32, 0) & 1) != 0) {
            t4 = Value1(Math_Sine, (FIELD_AT((a0 + 0x62), u8, 0) << 10));
            FIELD_AT(a0, s32, 76) = (FIELD_AT(((a0 + 0x64) - 1), u8, 0) * t4);
            FIELD_AT(((a0 + 0x64) - 1), u8, 0) = (FIELD_AT(((a0 + 0x64) - 1), u8, 0) + 0xa);
        }
    }
    if (FIELD_AT((0x66 + a0), u16, 0) != 0) {
        if (FIELD_AT(((a0 + 0x64) - 1), u8, 0) == 0x8d) {
            FIELD_AT((0x66 + a0), s16, 0) = (FIELD_AT((0x66 + a0), s16, 0) - 0x80);
        }
    }
    return 1;
}

