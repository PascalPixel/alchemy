#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 Map_GetTerrainHeight();
s32 Func_020057c4();
s32 Func_020059ec();

void Func_020040a0(void)
{
    s32 t0;
    s32 t1;

    t0 = Object_GetById();
    FIELD_AT((t0 + 0x55), u8, 0) = 4;
    FIELD_AT(t0, s32, 68) = 0;
    FIELD_AT(t0, s32, 12) = (FIELD_AT(t0, s32, 20) + 0x200000);
    FIELD_AT(((t0 + 0x55) - 0x32), u8, 0) = (0x80 | FIELD_AT(((t0 + 0x55) - 0x32), u8, 0));
    t1 = Value3(Map_GetTerrainHeight, FIELD_AT((t0 + 0x22), u8, 0), FIELD_AT(t0, s32, 8), FIELD_AT(t0, s32, 16));
    Call4(Func_020057c4, FIELD_AT(t0, s32, 8), FIELD_AT(t0, s32, 16), 0, ((t1 >> 19) + 6));
    Call4(Func_020059ec, FIELD_AT(t0, s32, 8), FIELD_AT(t0, s32, 16), 0, 0x80);
}

