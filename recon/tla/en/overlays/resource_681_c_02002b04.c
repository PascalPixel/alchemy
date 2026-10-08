#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Object_SetModeById();

void Func_02002b04(s32 a0, s32 a1)
{
    s32 t0;

    t0 = Object_GetById();
    FIELD_AT((t0 + 0x55), u8, 0) = 0;
    FIELD_AT((t0 + 0x23), u8, 0) = (0x80 | FIELD_AT((t0 + 0x23), u8, 0));
    FIELD_AT(t0, s32, 52) = 0x1000;
    FIELD_AT(t0, s32, 48) = 0x10000;
    FIELD_AT(t0, s32, 12) = 0x100000;
    FIELD_AT((t0 + 0x64), u16, 0) = FIELD_AT(t0, s16, 10);
    FIELD_AT((t0 + 0x66), u16, 0) = FIELD_AT(t0, s16, 18);
    Call2(Object_SetModeById, a0, ((((u32)(a1 << 24) >> 24) << 24) >> 24));
}

