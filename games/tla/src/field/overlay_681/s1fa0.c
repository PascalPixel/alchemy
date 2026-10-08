#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 GameFlag_ClearBit();
s32 Func_02004f8c();

void Func_02001fa0(void)
{
    s32 t0;

    t0 = Value1(Object_GetById, 0x42);
    FIELD_AT(t0, s32, 8) = 0x2280000;
    FIELD_AT(t0, s32, 16) = 0x1280000;
    FIELD_AT(t0, s32, 12) = -1966080;
    FIELD_AT(t0, s32, 20) = -2097152;
    Call1(GameFlag_ClearBit, 0x142);
    Call1(Func_02004f8c, 0x1042);
}

