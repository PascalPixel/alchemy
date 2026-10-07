#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
s32 Func_020019a4();

void Func_02001b60(void)
{
    Func_020019a4(0x59, 0, 0x4d, 0x10, 1, 2);
}

