#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 __divsi3();

s32 Func_020014d0(s32 a0)
{
    s32 t0;
    s32 t1;
    s32 t2;

    t0 = Value2(__divsi3, (FIELD_AT(FIELD_AT(a0, s32, 104), s32, 8) - FIELD_AT(a0, s32, 8)), 0xa);
    FIELD_AT(a0, s32, 68) = t0;
    t1 = Value2(__divsi3, ((FIELD_AT(FIELD_AT(a0, s32, 104), s32, 12) - FIELD_AT(a0, s32, 12)) + 0x100000), 0xa);
    FIELD_AT(a0, s32, 76) = t1;
    t2 = Value2(__divsi3, (FIELD_AT(FIELD_AT(a0, s32, 104), s32, 16) - FIELD_AT(a0, s32, 16)), 0xa);
    FIELD_AT(a0, s32, 72) = t2;
    FIELD_AT((a0 + 0x62), u8, 0) = 0xa;
    return 0;
}

