#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Vector_AddPolarOffsetFar();
s32 Func_020006a4();

void Func_020006f8(s32 a0)
{
    if (FIELD_AT(a0, s32, 76) >= FIELD_AT(a0, s32, 48)) {
        FIELD_AT((a0 + 0x64), u16, 0) = (FIELD_AT((a0 + 0x64), u16, 0) + ((FIELD_AT((0x66 + a0), u16, 0) << 16) >> 17));
        FIELD_AT(a0, s32, 8) = FIELD_AT(a0, s32, 68);
        FIELD_AT(a0, s32, 12) = FIELD_AT(a0, s32, 52);
        FIELD_AT(a0, s32, 16) = FIELD_AT(a0, s32, 72);
        Call3(Vector_AddPolarOffsetFar, FIELD_AT(a0, s32, 76), FIELD_AT((a0 + 0x64), u16, 0), (a0 + 8));
        Call1(Func_020006a4, a0);
        FIELD_AT(a0, s32, 76) = (FIELD_AT(a0, s32, 76) - (FIELD_AT(a0, s32, 48) >> 1));
        FIELD_AT((a0 + 0x64), u16, 0) = (FIELD_AT((a0 + 0x64), u16, 0) + ((FIELD_AT((0x66 + a0), u16, 0) << 16) >> 17));
        FIELD_AT((a0 + 8), s32, 0) = FIELD_AT(a0, s32, 68);
        FIELD_AT(a0, s32, 12) = FIELD_AT(a0, s32, 52);
        FIELD_AT(a0, s32, 16) = FIELD_AT(a0, s32, 72);
        Call3(Vector_AddPolarOffsetFar, FIELD_AT(a0, s32, 76), FIELD_AT((a0 + 0x64), u16, 0), (a0 + 8));
        Call1(Func_020006a4, a0);
        FIELD_AT(a0, s32, 76) = (FIELD_AT(a0, s32, 76) - (FIELD_AT(a0, s32, 48) >> 1));
        FIELD_AT(a0, s32, 52) = (FIELD_AT(a0, s32, 52) - FIELD_AT(a0, s32, 20));
    }
}

