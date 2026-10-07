#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_020019a4();

void Func_02001b44(void)
{
    Func_020019a4(0x57, 2, 0x50, 7, 1, 2);
}

