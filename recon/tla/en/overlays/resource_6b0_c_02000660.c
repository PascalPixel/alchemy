#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Vector_AddPolarOffsetFar();

void Func_02000660(s32 a0)
{
    FIELD_AT((a0 + 0x64), u16, 0) = (FIELD_AT((a0 + 0x64), u16, 0) + FIELD_AT((a0 + 0x66), u16, 0));
    FIELD_AT(a0, s32, 8) = FIELD_AT(a0, s32, 68);
    FIELD_AT(a0, s32, 12) = FIELD_AT(a0, s32, 72);
    FIELD_AT(a0, s32, 16) = 0x980000;
    Call3(Vector_AddPolarOffsetFar, FIELD_AT(a0, s32, 76), FIELD_AT((a0 + 0x64), u16, 0), (a0 + 8));
    FIELD_AT(a0, s32, 76) = (FIELD_AT(a0, s32, 76) + -65536);
}

